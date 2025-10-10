#ifndef QUANTUMCIRCUITPARALLEL_H
#define QUANTUMCIRCUITPARALLEL_H

#include "QuantumCircuitBase.h"

using namespace std;

class QuantumCircuitParallel : public QuantumCircuitBase {
public:
    //Constructor
    QuantumCircuitParallel(int n);

    void H(int target_qubit) override;
    void X(int target_qubit) override;
    void Y(int target_qubit) override;
    void Z(int target_qubit) override;
    void CNOT(int control_qubit, int target_qubit) override;
    void S(int target_qubit) override;
    void T(int target_qubit) override;
    void Tdg(int target_qubit) override;
};

#endif