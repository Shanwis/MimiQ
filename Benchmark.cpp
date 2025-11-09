#include <iostream>
#include <vector>
#include <random> 
#include <omp.h>

#include "./include/QuantumCircuitParallel.h"
#include "./include/QuantumCircuitMPI.h"

using namespace std;

// A simple struct to hold a gate operation for our random circuit
enum GateType { H, X, Y, Z, S, T, Tdg, CX };
struct GateOp {
    GateType type;
    int target;
    int control;
};

// Function to apply a specific gate from our GateOp list
void apply_gate_op(QuantumCircuitBase& qc, const GateOp& op) {
    switch (op.type) {
        case H: qc.H(op.target); break;
        case X: qc.X(op.target); break;
        case Y: qc.Y(op.target); break;
        case Z: qc.Z(op.target); break;
        case S: qc.S(op.target); break;
        case T: qc.T(op.target); break;
        case Tdg: qc.Tdg(op.target); break;
        case CX: qc.CX(op.control, op.target); break;
    }
}

int main() {
    int num_qubits;
    int num_threads;
    int num_gates = 500;

    cout << "--- Quantum Simulator Benchmark ---\n";
    cout << "Enter the number of qubits (e.g., 10): ";
    cin >> num_qubits;
    cout << "Enter the number of threads for the parallel version: ";
    cin >> num_threads;

    if (cin.fail() || num_qubits <= 0 || num_threads <= 0) {
        cerr << "Invalid input. Exiting.\n";
        return 1;
    }
    cout << "\nPreparing to run a random circuit with " << num_gates << " gates on a " << num_qubits << "-qubit system.\n";

    //The same random circuit for both cases
    vector<GateOp> random_circuit;
    random_circuit.reserve(num_gates);

    // Setup random number generation
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> gate_dist(0, 7); 
    uniform_int_distribution<> qubit_dist(0, num_qubits - 1);

    for (int i = 0; i < num_gates; ++i) {
        GateType type = static_cast<GateType>(gate_dist(gen));
        if (type == CX) {
            int control = qubit_dist(gen);
            int target = qubit_dist(gen);
            // Ensure control and target are not the same
            while (control == target) {
                target = qubit_dist(gen);
            }
            random_circuit.push_back({type, target, control});
        } else {
            random_circuit.push_back({type, qubit_dist(gen), -1});
        }
    }

    // ---PARALLEL VERSION ---
    cout << "\n--- Running Parallel Benchmark (" << num_threads << " threads) ---\n";
    QuantumCircuitParallel qc_parallel(num_qubits);
    omp_set_num_threads(num_threads);
    
    double start_parallel = omp_get_wtime();
    
    for (const auto& op : random_circuit) {
        apply_gate_op(qc_parallel, op);
    }
    
    double end_parallel = omp_get_wtime();
    double parallel_time = end_parallel - start_parallel;
    cout << "Parallel execution time: " << parallel_time << " seconds\n";

     // ---SERIAL VERSION ---
    cout << "\n--- Running Serial Benchmark ---\n";
    QuantumCircuitBase qc_serial(num_qubits);
    double start_serial = omp_get_wtime();

    for (const auto& op : random_circuit) {
        apply_gate_op(qc_serial, op);
    }

    double end_serial = omp_get_wtime();
    double serial_time = end_serial - start_serial;
    cout << "Serial execution time: " << serial_time << " seconds\n";

    // ---CALCULATE AND DISPLAY SPEEDUP ---
    if (parallel_time > 0) {
        double speedup = serial_time / parallel_time;
        cout << "\n-----------------------------------------------------------\n";
        cout << "Speedup: " << speedup << "x\n";
        cout << "--By Ashwin S, 2023BCS0044 & Elhan B Thomas, 2023BCS0119--\n";
    }

    return 0;
}