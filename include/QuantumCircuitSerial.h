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
    void CX(int control_qubit, int target_qubit) override;
    void CZ(int control_qubit, int target_qubit);
    void CH(int control_qubit, int target_qubit);
    void CY(int control_qubit, int target_qubit);
    void S(int target_qubit) override;
    void T(int target_qubit) override;
    void Tdg(int target_qubit) override;
    void Rz(int target_qubit, const double theta) override;
    void Rx(int target_qubit, const double theta) override;
    void Ry(int target_qubit, const double theta) override;
    void CRx(int control_qubit, int target_qubit, const double theta) override;
    void CRy(int control_qubit, int target_qubit, const double theta) override;
    void CRz(int control_qubit, int target_qubit, const double theta) override;

private:
    void applySingleQubitOp(int target_qubit, function<void(complex<double>&,complex<double>&)> op);
    void applyControlledQubitOp(int control_qubit, int target_qubit, function<void(complex<double>&, complex<double>&)> op);
};


#endif