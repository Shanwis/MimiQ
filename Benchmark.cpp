#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <cmath>
#include <omp.h>

#include <MaQrel/QuantumCircuitParallel.h>
#include <MaQrel/QuantumCircuitMPI.h>
#include <MaQrel/QuantumGates.h>

using namespace std;

// ---- Gate types (extended for parameterized gates) ----
enum GateType { H, X, Y, Z, S, T, Tdg, CX, RX, RZ, CP };
struct GateOp {
    GateType type;
    int target;
    int control;
    double theta;
};

void apply_gate_op(QuantumCircuitBase& qc, const GateOp& op) {
    switch (op.type) {
        case H:   qc.H(op.target); break;
        case X:   qc.X(op.target); break;
        case Y:   qc.Y(op.target); break;
        case Z:   qc.Z(op.target); break;
        case S:   qc.S(op.target); break;
        case T:   qc.T(op.target); break;
        case Tdg: qc.Tdg(op.target); break;
        case CX:  qc.CX(op.control, op.target); break;
        case RX:  qc.Rx(op.target, op.theta); break;
        case RZ:  qc.Rz(op.target, op.theta); break;
        case CP:  qc.CP(op.control, op.target, op.theta); break;
    }
}

GateMatrix gate_to_matrix(const GateOp& op) {
    switch (op.type) {
        case H:   return GateMatrices::H();
        case X:   return GateMatrices::X();
        case Y:   return GateMatrices::Y();
        case Z:   return GateMatrices::Z();
        case S:   return GateMatrices::S();
        case T:   return GateMatrices::T();
        case Tdg: return GateMatrices::Tdg();
        case RX:  return GateMatrices::Rx(op.theta);
        case RZ:  return GateMatrices::Rz(op.theta);
        default:  return GateMatrices::I();
    }
}

bool is_controlled(GateType t) { return t == CX || t == CP; }

vector<pair<int, GateMatrix>> fuse_gates(const vector<pair<int, GateMatrix>>& gates) {
    vector<pair<int, GateMatrix>> fused;
    for (auto& [t, m] : gates) {
        if (!fused.empty() && fused.back().first == t)
            fused.back().second = multiply(m, fused.back().second);
        else
            fused.push_back({t, m});
    }
    return fused;
}

// ---- Circuit generators ----

vector<GateOp> generate_random(int n, int n_gates, mt19937& gen) {
    uniform_int_distribution<> gate_dist(0, 7);
    uniform_int_distribution<> qubit_dist(0, n - 1);
    vector<GateOp> circuit;
    circuit.reserve(n_gates);
    for (int i = 0; i < n_gates; ++i) {
        GateType t = static_cast<GateType>(gate_dist(gen));
        if (t == CX) {
            int c = qubit_dist(gen);
            int q = qubit_dist(gen);
            while (c == q) q = qubit_dist(gen);
            circuit.push_back({t, q, c, 0});
        } else {
            circuit.push_back({t, qubit_dist(gen), -1, 0});
        }
    }
    return circuit;
}

vector<GateOp> generate_qaoa(int n, int layers, mt19937& gen) {
    uniform_real_distribution<double> angle_dist(0, 2 * M_PI);
    vector<GateOp> circuit;
    for (int l = 0; l < layers; ++l) {
        double theta = angle_dist(gen);
        double phi = angle_dist(gen);
        for (int q = 0; q < n; ++q) {
            circuit.push_back({RX, q, -1, theta});
            circuit.push_back({RZ, q, -1, phi});
        }
        for (int q = 0; q < n - 1; ++q)
            circuit.push_back({CX, q + 1, q, 0});
    }
    return circuit;
}

vector<GateOp> generate_qft(int n, int repeats) {
    vector<GateOp> circuit;
    for (int r = 0; r < repeats; ++r) {
        for (int i = 0; i < n; ++i) {
            circuit.push_back({H, i, -1, 0});
            for (int j = i + 1; j < n; ++j) {
                double theta = M_PI / (1ULL << (j - i));
                circuit.push_back({CP, i, j, theta});
            }
        }
        for (int i = 0; i < n / 2; ++i)
            circuit.push_back({CP, n - 1 - i, i, M_PI / 2});
    }
    return circuit;
}

// ---- Benchmarking helper ----

struct Result {
    double time;
    int orig_gates;
    int fused_gates;
    double speedup_vs_serial;
};

// ---- Main ----

int main() {
    int num_qubits, num_threads;
    cout << "=== MaQrel Benchmark ===\n";
    cout << "Qubits: ";  cin >> num_qubits;
    cout << "Threads: "; cin >> num_threads;
    if (cin.fail() || num_qubits <= 0 || num_threads <= 0) {
        cerr << "Invalid input.\n"; return 1;
    }
    omp_set_num_threads(num_threads);

    random_device rd;
    mt19937 gen(42);  // fixed seed for reproducibility

    // ---- Generate circuits ----
    auto rand_circuit = generate_random(num_qubits, 500, gen);
    auto qaoa_circuit = generate_qaoa(num_qubits, 50, gen);
    int qft_depth = num_qubits + num_qubits * (num_qubits - 1) / 2 + num_qubits / 2;
    int qft_repeats = max(1, (int)(500.0 / qft_depth));
    auto qft_circuit  = generate_qft(num_qubits, qft_repeats);

    struct CircuitDef {
        string name;
        vector<GateOp>& circuit;
    };
    CircuitDef circuits[] = {
        {"Random (500 gates)",      rand_circuit},
        {"QAOA-style (50 layers)",  qaoa_circuit},
        {"QFT (" + to_string(num_qubits) + " qubits, " + to_string(qft_repeats) + " repeats)", qft_circuit},
    };

    cout << "\n--- Results ---\n\n";

    for (auto& [name, circ] : circuits) {
        int total_1q = 0, total_cx = 0;
        for (auto& op : circ) {
            if (is_controlled(op.type)) total_cx++;
            else total_1q++;
        }
        int total = total_1q + total_cx;

        cout << "  " << name << "\n";
        cout << "  Total gates: " << total << "  (1-qubit: " << total_1q
             << ", controlled: " << total_cx << ")\n";

        // 1. Serial Lambda
        double t_serial;
        {
            QuantumCircuitBase qc(num_qubits);
            double t0 = omp_get_wtime();
            for (auto& op : circ) apply_gate_op(qc, op);
            t_serial = omp_get_wtime() - t0;
        }

        // 2. Parallel Lambda
        double t_plambda;
        {
            QuantumCircuitParallel qc(num_qubits);
            double t0 = omp_get_wtime();
            for (auto& op : circ) apply_gate_op(qc, op);
            t_plambda = omp_get_wtime() - t0;
        }

        // 3. Parallel Matrix (unfused)
        double t_matrix;
        {
            QuantumCircuitParallel qc(num_qubits, Matrix);
            double t0 = omp_get_wtime();
            for (auto& op : circ) {
                if (is_controlled(op.type)) {
                    if (op.type == CX) qc.CX(op.control, op.target);
                    else qc.CP(op.control, op.target, op.theta);
                } else {
                    qc.applyGate(op.target, gate_to_matrix(op));
                }
            }
            t_matrix = omp_get_wtime() - t0;
        }

        // 4. Parallel Matrix (fused)
        double t_fused;
        int n_orig_sq = 0, n_fused = 0;
        {
            vector<pair<int, GateMatrix>> sq_gates;
            for (auto& op : circ)
                if (!is_controlled(op.type))
                    sq_gates.push_back({op.target, gate_to_matrix(op)});
            n_orig_sq = sq_gates.size();
            auto fused = fuse_gates(sq_gates);
            n_fused = fused.size();

            QuantumCircuitParallel qc(num_qubits, Matrix);
            double t0 = omp_get_wtime();
            for (auto& [t, m] : fused)
                qc.applyGate(t, m);
            for (auto& op : circ)
                if (is_controlled(op.type)) {
                    if (op.type == CX) qc.CX(op.control, op.target);
                    else qc.CP(op.control, op.target, op.theta);
                }
            t_fused = omp_get_wtime() - t0;
        }

        // Print
        int red = n_orig_sq > 0 ? (int)((1.0 - (double)n_fused / n_orig_sq) * 100) : 0;
        cout << fixed << setprecision(6);
        cout << "    Serial Lambda    : " << t_serial << " s\n";
        cout << "    Parallel Lambda  : " << t_plambda << " s  ("
             << setprecision(2) << t_serial / t_plambda << "x)\n";
        cout << "    Parallel Matrix  : " << setprecision(6) << t_matrix << " s  ("
             << setprecision(2) << t_serial / t_matrix << "x)\n";
        cout << "    Parallel Fused   : " << setprecision(6) << t_fused << " s  ("
             << setprecision(2) << t_serial / t_fused << "x)"
             << "  [" << n_orig_sq << "->" << n_fused << ", -" << red << "%]\n";
        cout << "\n";
    }

    cout << "--By Ashwin S, 2023BCS0044 & Elhan B Thomas, 2023BCS0119--\n";
    return 0;
}
