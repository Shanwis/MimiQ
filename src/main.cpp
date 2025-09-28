#include "../include/QuantumCircuit.h" 
#include <iostream>
#include <ctype.h>
using namespace std;

// int main() {
//     cout << "Creating a 2-qubit circuit." << endl;
//     QuantumCircuit qc(2);

//     cout << "Applying Hadamard gate to qubit 0." << endl;
//     qc.H(0);

//     cout << "Applying NOT (X) gate to qubit 1." << endl;
//     qc.X(1);

//     cout << "Measuring the circuit..." << endl;
//     qc.measure();

//     return 0;
// }

int main() {
    cout << "----------------QUANTUM SIMULATOR-----------\n";
    cout << "Enter number of Qubits: ";
    int n;
    cin >> n;
    char choice = 'n';
    QuantumCircuit qc(n);

    while(choice == 'n') {
        cout << endl << "Enter the aphabet of the Gate you wish to apply.\n";
        cout << "X GATE (X)\n";
        cout << "Z GATE (Z)\n";
        cout << "HADAMARD GATE (H)\n";
        cout << "Your gate: ";
        char gate;
        cin >> gate;
        switch(toupper(gate)) {
            case 'X': 
            case 'Z':
            case 'H': cout << endl; break;
            default: cout << "Invalid Gate!!\n"; continue;
        }
        int bit;
        cout << "Enter the qubit number: ";
        cin  >> bit;
        if(bit > n) {
            cout << "Entered number exeeded number of qubits!!";
            continue;
        }

        switch(toupper(gate)) {
            case 'X': qc.X(bit); break;
            case 'Z': qc.Z(bit); break;  
            case 'H': qc.H(bit); break;
        }

        cout << "Measure Qubits?(y/n): ";
        cin >> choice;
        if(choice == 'y') qc.measure();
    }
}

