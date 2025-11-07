#include <omp.h>
#include <stdexcept>
#include <cmath>
#include "../include/QuantumCircuitParallel.h"
using namespace std;

const complex<double> I(0.0,1.0);

QuantumCircuitParallel::QuantumCircuitParallel(int n) : QuantumCircuitBase(n) {}

//Function for applying single qubit operations

void QuantumCircuitParallel::applySingleQubitOp(int target_qubit, function<void(complex<double>&,complex<double>&)> op){
    if(target_qubit<0 || target_qubit>=qubit_count) throw out_of_range("Target qubit is out of range");

    size_t block_size = 1<<target_qubit;
    size_t stride = 1<<(target_qubit+1);

    #pragma omp parallel for
    for(int i=0;i<state_vector.size();i+=stride){
        for(int j=0;j<block_size;j++){
            op(state_vector[i+j],state_vector[i+j+block_size]);
        }
    }
}

//Function applying phase

void QuantumCircuitParallel::applyPhase(int target_qubit,const complex<double>& phase){
    auto op = [&](auto &a, auto &b){
        b*=phase;
    };

    applySingleQubitOp(target_qubit,op);
}

//Funcion for applying controlled operations

void QuantumCircuitParallel::applyControlledQubitOp(int control_qubit, int target_qubit, function<void(complex<double>&, complex<double>&)> op){
    if(control_qubit >= qubit_count || control_qubit < 0 || target_qubit >= qubit_count || target_qubit <0 || control_qubit == target_qubit) throw out_of_range("Qubits out of range.");

    size_t control_mask = 1 << control_qubit;
    size_t block_size = 1 << target_qubit;
    size_t stride = 1<<(target_qubit+1);

    #pragma omp parallel for
    for(int i=0;i<state_vector.size();i+=stride){
        for(int j=0;j<block_size;j++){
            if(((i+j)&control_mask)!=0) op(state_vector[i+j],state_vector[i+j+block_size]);
        }
    }
}

//Type 1: Pauli Gates

void QuantumCircuitParallel::X(int target_qubit) {
    applySingleQubitOp(target_qubit,[](auto &a, auto &b){swap(a,b);});
    addCircuit(target_qubit, 'X', -1);
}

void QuantumCircuitParallel::Y(int target_qubit){
    applySingleQubitOp(target_qubit,[](auto &a, auto &b){
        complex<double> b_old = b;
        b=I*a;
        a=-I*b_old;}
    );
    addCircuit(target_qubit, 'Y', -1);
}

void QuantumCircuitParallel::Z(int target_qubit){
    applyPhase(target_qubit,(complex<double>)-1.0);
    addCircuit(target_qubit, 'Z', -1);
}

//Type 2: Superposition Gate

void QuantumCircuitParallel::H(int target_qubit){
    applySingleQubitOp(target_qubit,[](auto &a, auto &b){
        complex<double> a_old = a;
        complex<double> b_old = b;
        a=(a_old+b_old)/sqrt(2);
        b=(a_old-b_old)/sqrt(2);}
    );
    addCircuit(target_qubit, 'H', -1);
}

//Type 3: Phase Gate 

void QuantumCircuitParallel::S(int target_qubit){
    applyPhase(target_qubit,I);
    addCircuit(target_qubit, 'S', -1);
}

void QuantumCircuitParallel::T(int target_qubit) {
    applyPhase(target_qubit,polar(1.0, M_PI / 4.0));
    addCircuit(target_qubit, 'T', -1);
}

void QuantumCircuitParallel::Tdg(int target_qubit) {
    applyPhase(target_qubit,polar(1.0, -M_PI / 4.0));
    addCircuit(target_qubit, 't', -1);
}

void QuantumCircuitParallel::Rz(int target_qubit, const double theta){
    complex<double> p =I*theta;
    applyPhase(target_qubit,exp(p));
    addCircuit(target_qubit,'R',-1);
}

//Type 4: Entangling gate

void QuantumCircuitParallel::CX(int control_qubit, int target_qubit){
    applyControlledQubitOp(control_qubit,target_qubit, [](auto& a, auto& b){swap(a,b);});
    addCircuit(control_qubit, 'C', target_qubit);
}

void QuantumCircuitParallel::CZ(int control_qubit, int target_qubit){
    auto op = [](complex<double> &a, complex<double> &b){
        b*=-1.0;
    };

    applyControlledQubitOp(control_qubit,target_qubit, op);
    addCircuit(control_qubit, 'C', target_qubit);
}

void QuantumCircuitParallel::CY(int control_qubit, int target_qubit){
    auto op = [](complex<double> &a, complex<double> &b){
        complex<double> b_old = b;
        b = I*a;
        a = -I*b_old;
    };

    applyControlledQubitOp(control_qubit,target_qubit, op);
    addCircuit(control_qubit, 'C', target_qubit);
}

void QuantumCircuitParallel::CH(int control_qubit, int target_qubit){
    auto op = [](complex<double> &a, complex<double> &b){
        complex<double> a_old = a;
        complex<double> b_old = b;
        a = (a_old+b_old)/sqrt(2);
        b = (a_old-b_old)/sqrt(2);
    };

    applyControlledQubitOp(control_qubit,target_qubit, op);
    addCircuit(control_qubit, 'C', target_qubit);
}
