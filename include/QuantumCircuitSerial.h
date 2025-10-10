#ifndef QUANTUMCIRCUITSERIAL_H
#define QUANTUMCIRCUITSERIAL_H

#include "QuantumCircuitBase.h"

using namespace std;

class QuantumCircuitSerial : public QuantumCircuitBase {
public:
    //Constructor
    QuantumCircuitSerial(int n);
    void H(int target_qubit) override;
    void X(int target_qubit) override;
    void Y(int target_qubit) override;
    void Z(int target_qubit) override;
    void CNOT(int control_qubit, int target_qubit) override;
    void S(int target_qubit);
    void T(int target_qubit);
    void Tdg(int target_qubit);
};

#endif