#include <MaQrel/QuantumCircuitParallel.h>
#include<iostream>

int main(){
    QuantumCircuitParallel qc(3);

    qc.H(0);

    qc.H(1);
    qc.CX(1,2);

    qc.CX(0,1);
    qc.H(0);

    int m0 = qc.measure_single_qubit(0);
    int m1 = qc.measure_single_qubit(1);

    if(m0 == 1) qc.Z(2);
    if(m1 == 1) qc.X(2);

    qc.printState();
}