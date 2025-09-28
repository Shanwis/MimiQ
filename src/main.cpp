#include <iostream>
#include <limits>
#include "../include/QuantumCircuit.h"

using namespace std;

// Function to handle gate application logic
void applyGate(QuantumCircuit &qc, int qubit_count) {
    cout << "\nEnter gate to apply (H, X, Z, C for CNOT): ";
    char gate_type;
    cin >> gate_type;

    int target = -1, control = -1;

    try {
        if (toupper(gate_type) == 'C') {
            cout << "Enter control qubit: ";
            cin >> control;
            cout << "Enter target qubit: ";
            cin >> target;
            qc.CNOT(control, target);
            cout << "Applied CNOT(" << control << ", " << target << ")" << endl;
        } else {
            cout << "Enter target qubit: ";
            cin >> target;
            if (toupper(gate_type) == 'H') {
                qc.H(target);
                cout << "Applied H on qubit " << target << endl;
            } else if (toupper(gate_type) == 'X') {
                qc.X(target);
                cout << "Applied X on qubit " << target << endl;
            } else if (toupper(gate_type) == 'Z') {
                qc.Z(target);
                cout << "Applied Z on qubit " << target << endl;
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

    QuantumCircuit qc(n);
    bool running = true;

    while (running) {
        cout << "\n--- Menu ---\n";
        cout << "1. Apply Gate\n";
        cout << "2. Display Circuit\n";
        cout << "3. Measure Final State\n";
        cout << "4. Print State Vector (Debug)\n";
        cout << "5. Exit\n";
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
                cout << "\nMeasuring the final state..." << endl;
                qc.measure();
                break;
            case 4:
                qc.printState();
                break;
            case 5:
                running = false;
                cout << "Exiting simulator. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please enter a number from 1 to 5." << endl;
                break;
        }
    }

    return 0;
}