#include <complex>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include "../include/QuantumCircuitSerial.h"
using namespace std;

// Constructor with member initializer list
QuantumCircuitSerial::QuantumCircuitSerial(int n) : QuantumCircuitBase(n) {}

//Pattern 1: Superposition/Swapping Gates

const complex<double> I(0.0,1.0);

void QuantumCircuitSerial::X(int target_qubit) {
    if(target_qubit >= qubit_count || target_qubit < 0) throw out_of_range("Qubit index out of range.");

    size_t stride = 1 << (target_qubit + 1);
    size_t block_size = 1 << target_qubit;

    for(size_t i=0; i<state_vector.size(); i+=stride){
        for(size_t j=0; j<block_size; j++){
            swap(state_vector[i+j], state_vector[i+j+block_size]); //X gate swaps amplitudes
        }
    }
    addCircuit(target_qubit, 'X', -1);
}

void QuantumCircuitSerial::H(int target_qubit){
    if(target_qubit >= qubit_count || target_qubit <0) throw out_of_range("Qubits index out of range.");

    const complex<double> inv_sq_2 = 1.0 / sqrt(2.0);
    size_t stride = 1 << (target_qubit + 1);
    size_t block_size = 1 << target_qubit;

    for(size_t i = 0; i<state_vector.size(); i+=stride){
        for(size_t j=0; j<block_size; ++j) {

            //applying Hadamard on relevent pairs
            complex<double> a = state_vector[i+j];
            complex<double> b = state_vector[i+j+block_size];
            state_vector[i+j] = inv_sq_2 * (a+b);
            state_vector[i+j+block_size] = inv_sq_2 * (a-b);
        }
    }
    addCircuit(target_qubit, 'H', -1);
}

void QuantumCircuitSerial::Y(int target_qubit){
    if(target_qubit >= qubit_count || target_qubit <0) throw out_of_range("Qubits index out of range.");
    
    size_t stride = 1 << (target_qubit + 1);
    size_t block_size = 1 << target_qubit;

    for(size_t i = 0; i<state_vector.size(); i+=stride){
        for(size_t j=0; j<block_size; ++j) {

            //applying Y on relevent pairs
            complex<double> a = state_vector[i+j];
            complex<double> b = state_vector[i+j+block_size];
            state_vector[i+j] = -I * b;
            state_vector[i+j+block_size] = I * a;
        }
    }
    addCircuit(target_qubit, 'Y', -1);
}

//Pattern 2: Phase Gates

void QuantumCircuitSerial::Z(int target_qubit){
    if(target_qubit >= qubit_count || target_qubit <0) throw out_of_range("Qubits index out of range.");

    size_t mask = 1<< target_qubit;
    for(size_t i = 0; i < state_vector.size(); ++i){
        if((i&mask) != 0){
            //apply -1 phase
            state_vector[i] *= -1.0;
        }
    }
    addCircuit(target_qubit, 'Z', -1);
}

void QuantumCircuitSerial::S(int target_qubit){
    if(target_qubit >= qubit_count || target_qubit <0) throw out_of_range("Qubits index out of range.");
    size_t mask = 1<< target_qubit;
    for(size_t i = 0; i < state_vector.size(); ++i){
        if((i&mask) != 0){
            //apply phase of I
            state_vector[i] *= I;
        }
    }
    addCircuit(target_qubit, 'S', -1);
}

void QuantumCircuitSerial::T(int target_qubit) {
    if (target_qubit < 0 || target_qubit >= qubit_count) {
        throw std::out_of_range("Target qubit is out of range.");
    }

    const std::complex<double> phase = std::polar(1.0, M_PI / 4.0);

    for (size_t i = 0; i < state_vector.size(); ++i) {
        if ((i >> target_qubit) & 1) {
            state_vector[i] *= phase;
        }
    }

    addCircuit(target_qubit, 'T', -1);
}

void QuantumCircuitSerial::Tdg(int target_qubit) {
    if (target_qubit < 0 || target_qubit >= qubit_count) {
        throw std::out_of_range("Target qubit is out of range.");
    }

    const std::complex<double> phase = std::polar(1.0, -M_PI / 4.0);

    for (size_t i = 0; i < state_vector.size(); ++i) {
        if ((i >> target_qubit) & 1) {
            state_vector[i] *= phase;
        }
    }

    addCircuit(target_qubit, 't', -1);
}

//Pattern 3: Two-Qubit Gates

void QuantumCircuitSerial::CNOT(int control_qubit, int target_qubit){
    if(control_qubit >= qubit_count || control_qubit < 0 || target_qubit >= qubit_count || target_qubit <0 || control_qubit == target_qubit) throw out_of_range("Qubits out of range.");

    size_t control_mask = 1 << control_qubit;
    size_t target_mask = 1 << target_qubit;

    for(size_t i=0; i<state_vector.size(); ++i) {
        if((i&control_mask) != 0){
            if(i < (i^target_mask)) swap(state_vector[i], state_vector[i^target_mask]);
        }
    }
    addCircuit(control_qubit, 'C', target_qubit);
}