#ifndef QUANTUMCIRCUITPARALLEL_H
#define QUANTUMCIRCUITPARALLEL_H

#include "QuantumCircuitBase.h"

class QuantumCircuitParallel : public QuantumCircuitBase {
public:
    //Constructor
    QuantumCircuitParallel(int n);

private:
// here we are overriding to include openMP
    void applySingleQubitOp(int target_qubit, std::function<void(std::complex<double>&,std::complex<double>&)> op) override; 
    void applyControlledQubitOp(int control_qubit, int target_qubit, std::function<void(std::complex<double>&, std::complex<double>&)> op) override;
};

#endif