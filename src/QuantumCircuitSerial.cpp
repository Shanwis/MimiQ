#include <complex>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include "../include/QuantumCircuitSerial.h"
#include "../include/QuantumGates.h"
using namespace std;

QuantumCircuitSerial::QuantumCircuitSerial(int n) : QuantumCircuitBase(n) {}

//Function for applying single qubit operations

void QuantumCircuitSerial::applySingleQubitOp(int target_qubit, function<void(complex<double>&,complex<double>&)> op){
    if(target_qubit<0 || target_qubit>=qubit_count) throw out_of_range("Target qubit is out of range");

    size_t block_size = 1<<target_qubit;
    size_t stride = 1<<(target_qubit+1);

    for(int i=0;i<state_vector.size();i+=stride){
        for(int j=0;j<block_size;j++){
            op(state_vector[i+j],state_vector[i+j+block_size]);
        }
    }
}

//Funcion for applying controlled operations

void QuantumCircuitSerial::applyControlledQubitOp(int control_qubit, int target_qubit, function<void(complex<double>&, complex<double>&)> op){
    if(control_qubit >= qubit_count || control_qubit < 0 || target_qubit >= qubit_count || target_qubit <0 || control_qubit == target_qubit) throw out_of_range("Qubits out of range.");

    size_t control_mask = 1 << control_qubit;
    size_t block_size = 1 << target_qubit;
    size_t stride = 1<<(target_qubit+1);

    for(int i=0;i<state_vector.size();i+=stride){
        for(int j=0;j<block_size;j++){
            if(((i+j)&control_mask)!=0) op(state_vector[i+j],state_vector[i+j+block_size]);
        }
    }
}

//Type 1: Pauli Gates

void QuantumCircuitSerial::X(int target_qubit) {
    applySingleQubitOp(target_qubit,QuantumGates::X_Function());
    addCircuit(target_qubit, "X");
}

void QuantumCircuitSerial::Y(int target_qubit){
    applySingleQubitOp(target_qubit,QuantumGates::Y_Function());
    addCircuit(target_qubit, "Y");
}

void QuantumCircuitSerial::Z(int target_qubit){
    // applyPhase(target_qubit,(complex<double>)-1.0);
    applySingleQubitOp(target_qubit,QuantumGates::Z_Function());
    addCircuit(target_qubit, "Z");
}

//Type 2: Superposition Gate

void QuantumCircuitSerial::H(int target_qubit){
    applySingleQubitOp(target_qubit,QuantumGates::H_Function());
    addCircuit(target_qubit, "H");
}

//Type 3: Phase Gate 

void QuantumCircuitSerial::S(int target_qubit){
    applySingleQubitOp(target_qubit,QuantumGates::Phase_Function(QuantumGates::I));
    addCircuit(target_qubit, "S");
}

void QuantumCircuitSerial::T(int target_qubit) {
    // applyPhase(target_qubit,polar(1.0, M_PI / 4.0));
    applySingleQubitOp(target_qubit,QuantumGates::Phase_Function(polar(1.0, M_PI / 4.0)));
    addCircuit(target_qubit, "T");
}

void QuantumCircuitSerial::Tdg(int target_qubit) {
    // applyPhase(target_qubit,polar(1.0, -M_PI / 4.0));
    applySingleQubitOp(target_qubit,QuantumGates::Phase_Function(polar(1.0, -M_PI / 4.0)));
    addCircuit(target_qubit, "Tdg");
}

void QuantumCircuitSerial::Rz(int target_qubit, const double theta){
    applySingleQubitOp(target_qubit,QuantumGates::Rz_Function(theta));
    addCircuit(target_qubit,"Rz("+to_string(theta)+")");
}

void QuantumCircuitSerial::Rx(int target_qubit, const double theta){
    applySingleQubitOp(target_qubit,QuantumGates::Rx_Function(theta));
    addCircuit(target_qubit,"Rx("+to_string(theta)+")");
}

void QuantumCircuitSerial::Ry(int target_qubit, const double theta){
    applySingleQubitOp(target_qubit,QuantumGates::Ry_Function(theta));
    addCircuit(target_qubit,"Ry("+to_string(theta)+")");
}

//Type 4: Entangling gate

void QuantumCircuitSerial::CX(int control_qubit, int target_qubit){
    applyControlledQubitOp(control_qubit,target_qubit, QuantumGates::X_Function());
    addCircuit(control_qubit, "C", target_qubit, "X");
}

void QuantumCircuitSerial::CZ(int control_qubit, int target_qubit){
    applyControlledQubitOp(control_qubit,target_qubit, QuantumGates::Z_Function());
    addCircuit(control_qubit, "C", target_qubit, "Z");
}

void QuantumCircuitSerial::CY(int control_qubit, int target_qubit){
    applyControlledQubitOp(control_qubit,target_qubit, QuantumGates::Y_Function());
    addCircuit(control_qubit, "C", target_qubit, "Y");
}

void QuantumCircuitSerial::CH(int control_qubit, int target_qubit){
    applyControlledQubitOp(control_qubit,target_qubit, QuantumGates::H_Function());
    addCircuit(control_qubit, "C", target_qubit, "H");
}

void QuantumCircuitSerial::CRz(int control_qubit, int target_qubit, const double theta){
    applyControlledQubitOp(control_qubit,target_qubit, QuantumGates::Rz_Function(theta));
    addCircuit(control_qubit, "C", target_qubit, "Rz("+to_string(theta)+")");
}

void QuantumCircuitSerial::CRx(int control_qubit, int target_qubit, const double theta){
    applyControlledQubitOp(control_qubit,target_qubit, QuantumGates::Rx_Function(theta));
    addCircuit(control_qubit, "C", target_qubit, "Rx("+to_string(theta)+")");
}

void QuantumCircuitSerial::CRy(int control_qubit, int target_qubit, const double theta){
    applyControlledQubitOp(control_qubit,target_qubit, QuantumGates::Ry_Function(theta));
    addCircuit(control_qubit, "C", target_qubit, "Ry("+to_string(theta)+")");
}
