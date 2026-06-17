#include <omp.h>
#include <stdexcept>
#include <cmath>
#include <MaQrel/QuantumCircuitParallel.h>
#include <MaQrel/QuantumGates.h>

using namespace std;

QuantumCircuitParallel::QuantumCircuitParallel(int n, SimMode m) : QuantumCircuitBase(n, m) {}

//Function for applying single qubit operations

void QuantumCircuitParallel::applySingleQubitOp(int target_qubit, function<void(complex<double>&,complex<double>&)> op){
    if(target_qubit<0 || target_qubit>=qubit_count) throw out_of_range("Target qubit is out of range");

    size_t block_size = 1ULL<<target_qubit;
    size_t stride = 1ULL<<(target_qubit+1);

    #pragma omp parallel for schedule(static)
    for(int i=0;i<state_vector.size();i+=stride){
        for(int j=0;j<block_size;j++){
            op(state_vector[i+j],state_vector[i+j+block_size]);
        }
    }
}

//Funcion for applying controlled operations

void QuantumCircuitParallel::applyControlledQubitOp(int control_qubit, int target_qubit, function<void(complex<double>&, complex<double>&)> op){
    if(control_qubit >= qubit_count || control_qubit < 0 || target_qubit >= qubit_count || target_qubit <0 || control_qubit == target_qubit) throw out_of_range("Qubits out of range.");

    size_t control_mask = 1ULL << control_qubit;
    size_t block_size = 1ULL << target_qubit;
    size_t stride = 1ULL <<(target_qubit+1);

    #pragma omp parallel for schedule(static)
    for(int i=0;i<state_vector.size();i+=stride){
        for(int j=0;j<block_size;j++){
            if(((i+j)&control_mask)!=0) op(state_vector[i+j],state_vector[i+j+block_size]);
        }
    }
}

void QuantumCircuitParallel::applySingleQubitMatrix(int target_qubit, const GateMatrix& g){
    if(target_qubit<0 || target_qubit>=qubit_count)
        throw out_of_range("Target qubit is out of range");

    size_t stride = 1ULL << target_qubit;
    size_t num_blocks = state_vector.size() >> (target_qubit + 1);
    size_t block_size = stride << 1;

    #pragma omp parallel for schedule(static)
    for(int block=0; block<(int)num_blocks; block++){
        size_t base = block * block_size;
        for(size_t j=0; j<stride; j++){
            size_t i0 = base + j;
            size_t i1 = i0 + stride;
            complex<double> a = state_vector[i0];
            complex<double> b = state_vector[i1];
            state_vector[i0] = g.m[0]*a + g.m[1]*b;
            state_vector[i1] = g.m[2]*a + g.m[3]*b;
        }
    }
}

void QuantumCircuitParallel::applyControlledQubitMatrix(int control_qubit, int target_qubit, const GateMatrix& g){
    if(control_qubit >= qubit_count || control_qubit < 0 ||
       target_qubit >= qubit_count || target_qubit < 0)
        throw out_of_range("Qubits out of range.");
    if(control_qubit == target_qubit)
        throw invalid_argument("Control and target qubits cannot be the same.");

    size_t control_mask = 1ULL << control_qubit;
    size_t stride = 1ULL << target_qubit;
    size_t num_blocks = state_vector.size() >> (target_qubit + 1);
    size_t block_size = stride << 1;

    #pragma omp parallel for schedule(static)
    for(int block=0; block<(int)num_blocks; block++){
        size_t base = block * block_size;
        for(size_t j=0; j<stride; j++){
            size_t i0 = base + j;
            size_t i1 = i0 + stride;
            if((i0 & control_mask) != 0){
                complex<double> a = state_vector[i0];
                complex<double> b = state_vector[i1];
                state_vector[i0] = g.m[0]*a + g.m[1]*b;
                state_vector[i1] = g.m[2]*a + g.m[3]*b;
            }
        }
    }
}
