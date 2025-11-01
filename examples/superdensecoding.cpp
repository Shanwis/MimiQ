#include "include/QuantumCircuitParallel.h"

int main(){
    QuantumCircuitParallel qc(2);

    qc.H(0);
    qc.CNOT(0,1);

    qc.X(0);

    qc.CNOT(0,1);
    qc.H(0);

    qc.printCircuit();
    qc.measureProbabilities();
    qc.displayGraph();
}