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
void updateCircuit(vector<vector<char>>& circuit, char gate, int bit) {
    for(int i=0;i<circuit.size();i++) { 
        if(i != bit) circuit[i].push_back('-');
        else circuit[i].push_back(gate);
    }
}

void displayCircuit(vector<vector<char>>& circuit) {
    for(int i=0;i<circuit.size();i++) {
        cout << endl;
        for(int j=0;j<circuit[i].size();j++){
            if(j>0){
                cout << circuit[i][j] << " ";
            }
            else{
                cout << "|0>  " << circuit[i][j] << " ";
            }
        }
    }
    cout << endl;
}

void applyGate(QuantumCircuit& qc, int n, vector<vector<char>>& circuit) {
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
            default: cout << "Invalid Gate!!\n"; return;
        }
        int bit;
        cout << "Enter the qubit number: ";
        cin  >> bit;
        cout << endl;
        if(bit > n) {
            cout << "Entered number exeeded number of qubits!!";
            return;
        }

        switch(toupper(gate)) {
            case 'X': qc.X(bit); updateCircuit(circuit, 'X', bit); break;
            case 'Z': qc.Z(bit); updateCircuit(circuit, 'Z', bit); break;  
            case 'H': qc.H(bit); updateCircuit(circuit, 'H', bit); break;
        }
}

int main() {
    cout << "-------QUANTUM SIMULATOR-------\n";
    cout << "Enter number of Qubits: ";
    int n;
    cin >> n;
    char choice = 'y';
    QuantumCircuit qc(n);
    vector<vector<char>> circuit(n);

    while(choice == 'y') {
        cout << "1. Apply Gate operation\n";
        cout << "2. Measure states\n";
        cout << "3.Display Circuit\n";
        cout << "Your choice: ";
        int ch;
        cin >> ch;
        
        switch(ch) {
            case 1: applyGate(qc, n, circuit); break;
            case 2: qc.measure(); choice = 'n'; break;
            case 3: displayCircuit(circuit);
        }
    }
}

