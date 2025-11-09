#ifndef QUANTUMCIRCUITPARALLEL_H
#define QUANTUMCIRCUITPARALLEL_H

#include "QuantumCircuitBase.h"

using namespace std;

class QuantumCircuitParallel : public QuantumCircuitBase {
public:
    //Constructor
    QuantumCircuitParallel(int n);

private:
    void applySingleQubitOp(int target_qubit, function<void(complex<double>&,complex<double>&)> op) override;
    void applyControlledQubitOp(int control_qubit, int target_qubit, function<void(complex<double>&, complex<double>&)> op) override;
};

#endif