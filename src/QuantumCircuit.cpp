#include <iostream>
#include<algorithm>
#include <cmath>
#include <iomanip>
#include <stdexcept>
#include "../include/QuantumCircuit.h"
using namespace std;

// Constructor with member initializer list
QuantumCircuit::QuantumCircuit(int n) :
    qubit_count(n)
{
    if(n<=0) {
        throw invalid_argument("Number of qubits must be positive.");
    }

    size_t state_size = 1<<n; //size is 2^n
    state_vector.resize(state_size,0);
    circuit.resize(qubit_count, "");

    state_vector[0] = 1.0; //Initialize the system to first state.
}

void QuantumCircuit::addCircuit(int qubit, char gate, int t_qubit = -1){
    for(int i=0; i<qubit_count; i++){
        if(i == qubit){ circuit[i] += "-[" + string(1,gate) + "]--"; }
        else if (i==t_qubit){ circuit[i] += "-[*]-"; }
        else if (i>qubit && i<t_qubit) { circuit[i] += "--+--";}
        else circuit[i] += "-----";
    }
}

void QuantumCircuit::printCircuit(){
    for(int i=0; i<qubit_count; i++){
        cout << i << " " << circuit[i] << "\n";
    }
}

void QuantumCircuit::X(int target_qubit) {
    if(target_qubit >= qubit_count || target_qubit < 0) throw out_of_range("Qubit index out of range.");

    size_t stride = 1 << (target_qubit + 1);
    size_t block_size = 1 << target_qubit;

    for(size_t i=0; i<state_vector.size(); i+=stride){
        for(size_t j=0; j<block_size; j++){
            swap(state_vector[i+j], state_vector[i+j+block_size]); //X gate swaps amplitudes
        }
    }
    addCircuit(target_qubit, 'X');
}

void QuantumCircuit::H(int target_qubit){
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
    addCircuit(target_qubit, 'H');
}

void QuantumCircuit::Z(int target_qubit){
    if(target_qubit >= qubit_count || target_qubit <0) throw out_of_range("Qubits index out of range.");

    size_t mask = 1<< target_qubit;
    for(size_t i = 0; i < state_vector.size(); ++i){
        if(i&mask != 0){
            //apply -1 phase
            state_vector[i] *= -1.0;
        }
    }
    addCircuit(target_qubit, 'Z');
}

const complex<double> I(0.0,1.0);

void QuantumCircuit::Y(int target_qubit){
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
    addCircuit(target_qubit, 'Y');
}


void QuantumCircuit::S(int target_qubit){
    if(target_qubit >= qubit_count || target_qubit <0) throw out_of_range("Qubits index out of range.");
    size_t mask = 1<< target_qubit;
    for(size_t i = 0; i < state_vector.size(); ++i){
        if(i&mask != 0){
            //apply phase of I
            state_vector[i] *= I;
        }
    }
    addCircuit(target_qubit, 'Z');
}

void QuantumCircuit::CNOT(int control_qubit, int target_qubit){
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

vector<string> QuantumCircuit::generateBasisStates(int n){
    vector<string> basis_states;
    size_t num_states = 1<<n;
    for(size_t i = 0; i<num_states; i++){
        string basis = "";
        int temp = i;
        for(int j=0; j<n; j++){
            basis += to_string(temp%2);
            temp/=2;
        }
        reverse(basis.begin(), basis.end());
        basis_states.push_back(basis);
    }
    return basis_states;
}


void QuantumCircuit::measure(){
    vector<string> basis_states = generateBasisStates(qubit_count);

    cout << fixed << setprecision(6);
    cout << qubit_count << "-Qubit Measurement Results" << "\n";
    for(size_t i = 0; i<state_vector.size(); ++i) {
        double prob = norm(state_vector[i]);
        cout << "Probability of |" << basis_states[i] << ">: " << prob << "\n";
    }
    cout << "----------------------------\n";
}

void QuantumCircuit::printState() {
    cout << "Current State Vector" << "\n";
    vector<string>basis_states = generateBasisStates(qubit_count);
    for(size_t i=0; i<state_vector.size(); i++){
        cout << "|" << basis_states[i] << "> :" << state_vector[i] << "\n";
    }
}


