#include <MaQrel/QuantumCircuitParallel.h>

int main(){
    QuantumCircuitParallel qc(2);

    qc.H(0);
    qc.CX(0,1);

    qc.printCircuit();
    qc.printState();

    qc.printProbabilities();
}