#include <iostream>
#include "../include/QuantumCircuit.h"

int main() {

    std::cout << "--- Creating a 2-Qubit Bell State |Φ+> ---" << std::endl;

    QuantumCircuit qc(2);

    qc.H(0);

    qc.CNOT(0, 1);

    qc.printState();

    qc.measure();

    return 0;
}