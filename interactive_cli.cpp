#include <iostream>
#include <limits>
#include <algorithm>
#include <MaQrel/QuantumCircuitParallel.h>
#include <MaQrel/QuantumCircuitBase.h>
using namespace std;

// Function to handle gate application logic
void applyGate(QuantumCircuitParallel &qc, int qubit_count) {
    cout << "\nEnter gate to apply (H, X, Z, Y, S, T, Tdg, C for CNOT, M for Measuring): ";
    string gate_type;
    cin >> gate_type;
    transform(gate_type.begin(), gate_type.end(), gate_type.begin(), ::toupper);

    int target = -1, control = -1;

    try {
        if (gate_type == "C") {
            cout << "Enter control qubit: ";
            cin >> control;
            cout << "Enter target qubit: ";
            cin >> target;
            qc.CX(control, target);
            cout << "Applied CNOT(" << control << ", " << target << ")" << endl;
        } else {
            cout << "Enter target qubit: ";
            cin >> target;
            if (gate_type == "H") {
                qc.H(target);
                cout << "Applied H on qubit " << target << endl;
            } else if (gate_type == "X") {
                qc.X(target);
                cout << "Applied X on qubit " << target << endl;
            } else if (gate_type == "Z") {
                qc.Z(target);
                cout << "Applied Z on qubit " << target << endl;
            } else if (gate_type == "Y"){
                qc.Y(target);
                cout << "Applied Y on qubit " << target << endl;
            }else if (gate_type == "T"){
                qc.T(target);
                cout << "Applied T on qubit " << target << endl;
            }else if (gate_type == "TDG"){
                qc.Tdg(target);
                cout << "Applied T dagger on qubit " << target << endl;
            }else if(gate_type == "M"){
                qc.measure_single_qubit(target);
            } else {
                cout << "Invalid gate type." << endl;
            }
        }
    } catch (const out_of_range& e) {
        cerr << "Error: " << e.what() << ". Qubit index must be between 0 and " << qubit_count - 1 << "." << endl;
    } catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

int main() {
    cout << "--- Quantum Circuit Simulator ---" << "\n";
    int n;
    cout << "Enter the number of qubits: ";
    cin >> n;

    if (cin.fail() || n <= 0) {
        cout << "Invalid number of qubits. Exiting." << endl;
        return 1;
    }

    QuantumCircuitParallel qc(n);
    bool running = true;

    while (running) {
        cout << "\n--- Menu ---\n";
        cout << "1. Apply Gate (includes measurement of single qubits)\n";
        cout << "2. Display Circuit\n";
        cout << "3. Measure Circuit(Collapses)\n";
        cout << "4. Print State Vector (Debug)\n";
        cout << "5. Print probabilities (Debug)\n";
        cout << "6. Display probability graph\n";
        cout << "7. Display heat map representation\n";
        cout << "8. Exit\n";
        cout << "Your choice: ";

        int choice;
        cin >> choice;

        if (cin.fail()) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
            continue;
        }

        switch (choice) {
            case 1:
                applyGate(qc, n);
                break;
            case 2:
                qc.printCircuit();
                break;
            case 3:
                cout << "\nMeasuring the current state." << endl;
                qc.collapse();
                break;
            case 4:
                qc.printState();
                break;
            case 5:
                cout << "\nProbabilities of states." << endl;
                qc.printProbabilities();
                break;
            case 6:
                qc.displayGraph();
                break;
            case 7:
                qc.displayHeatMap();
                break;
            case 8:
                running = false;
                cout << "Exiting simulator. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please enter a number from 1 to 7." << endl;
        }
    }

    return 0;
}