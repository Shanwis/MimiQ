#include "../include/QuantumCircuit.h" 
#include <iostream>
using namespace std;

int main() {
    cout << "Creating a 2-qubit circuit." << endl;
    QuantumCircuit qc(2);

    cout << "Applying Hadamard gate to qubit 0." << endl;
    qc.H(0);

    cout << "Applying NOT (X) gate to qubit 1." << endl;
    qc.X(1);

    cout << "Measuring the circuit..." << endl;
    qc.measure();

    return 0;
}