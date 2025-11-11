#ifndef QUANTUMCIRCUITMPI_H
#define QUANTUMCIRCUITMPI_H

#include "QuantumCircuitBase.h"


using namespace std;

class QuantumCircuitMPI : public QuantumCircuitBase {
public:
    //Constructor
    QuantumCircuitMPI(int n);

private:
    void applySingleQubitOp(int target_qubit, function<void(complex<double>&,complex<double>&)> op) override;
    void applyControlledQubitOp(int control_qubit, int target_qubit, function<void(complex<double>&, complex<double>&)> op) override;
};

#endif