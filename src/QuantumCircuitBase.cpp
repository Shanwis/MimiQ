#include <iostream>
#include <algorithm>
#include <random>
#include <cmath>
#include <map>
#include <stdexcept>
#include <MaQrel/QuantumCircuitBase.h>
#include <MaQrel/QuantumGates.h>
#include <MaQrel/QuantumVisualization.h>
using namespace std;

// Constructor with member initializer list
QuantumCircuitBase::QuantumCircuitBase(int n) :
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

double QuantumCircuitBase::expectZ(vector<int> &q){
    double expect = 0,normal;
    for(int i=0;i<state_vector.size();i++){
        int parity=0;
        for(int j:q){
            parity^=((i>>j)&1);
        }
        normal = norm(state_vector[i]);
        expect += (parity ? -normal:normal);
    }
    return expect;
}

void QuantumCircuitBase::addCircuit(int qubit, const string &gate){
    string box_name = "["+gate+"]";
    int gate_width = box_name.length();

    alignCircuitColumns();
    for(int i=0;i<qubit_count;i++){
        if(i==qubit) circuit[i]+=box_name;
        else circuit[i] += string(gate_width,'-');
    }
}

void QuantumCircuitBase::addCircuit(int qubit1,const string &gate1, int qubit2,const string &gate2){

    int max_gate_width = max(gate1.length(),gate2.length());
    string seperator = "-+" + string(max_gate_width+1,'-');

    string box_name_1 = "["+gate1+string(max_gate_width-gate1.length(),' ')+"]";
    string box_name_2 = "["+gate2+string(max_gate_width-gate2.length(),' ')+"]";

    alignCircuitColumns();

    for(int i=0;i<qubit_count;i++){

        if(i==qubit1) circuit[i]+=box_name_1;
        else if(i>qubit1 && i<qubit2) circuit[i]+=seperator;
        else if(i==qubit2) circuit[i]+=box_name_2;
        else circuit[i]+= string(max_gate_width+2,'-');
    }
}

void QuantumCircuitBase::alignCircuitColumns(){
    size_t max_length = 0;
    for(auto &line:circuit) max_length = max(max_length, line.length()); 
    for(auto &line:circuit) line += string(max_length-line.length(),'-');
}

void QuantumCircuitBase::printCircuit(){
    QuantumVisualization::printCircuit(circuit,qubit_count);
}

//collapse
string QuantumCircuitBase::collapse(){
    vector<string> basis_states = QuantumVisualization::generateBasisStates(qubit_count);
    vector<double> weights;
    for(auto &a:state_vector){
        weights.push_back(norm(a));
    }

    //Getting the state which it would be at
    static random_device rd;
    static mt19937 gen(rd());
    discrete_distribution<> dist(weights.begin(), weights.end());
    int index = dist(gen);
    
    resetAll(index);
    cout << basis_states[index] << "\n";
    for(int i=0; i<qubit_count; i++){
       circuit[i] += "[M]";
    }
    return basis_states[index];
}

string index_to_basis_string(size_t index, int qubit_count) {
    string basis_str(qubit_count, '0');
    for (int i = 0; i < qubit_count; ++i) {
        if ((index >> i) & 1) {
            basis_str[qubit_count - 1 - i] = '1';
        }
    }
    return basis_str;
}

map<string,int> QuantumCircuitBase::run(int num_shots){
    vector<double> probabilities;
    for(auto &amplitude:state_vector){
        probabilities.push_back(norm(amplitude));
    }

    static random_device rd;
    static mt19937 gen(rd());
    discrete_distribution<> dist(probabilities.begin(), probabilities.end());

    map<string,int> result;
    for(int i=0;i<num_shots;i++){
        int value = dist(gen);
        result[index_to_basis_string(value, qubit_count)]++;
    }

    for(int i=0; i<qubit_count; i++){
       circuit[i] += "[M]";
    }

    return result;
}

int QuantumCircuitBase::measure_single_qubit(int qubit){
    
    double prob_of_one = 0.0;
    size_t num_states = 1<<qubit_count;

    for(size_t i=0; i<num_states; i++){
        if((i>>qubit) & 1){
            prob_of_one += norm(state_vector[i]);
        }
    }

    static random_device rd;
    static mt19937 gen(rd());
    bernoulli_distribution dist(prob_of_one);
    int measurement = dist(gen);

    double norm_factor = measurement == 1 ? sqrt(prob_of_one) : sqrt(1.0-prob_of_one);

    for(int i=0; i<num_states; i++){
        bool bit_is_one = ((i>>qubit) & 1);

        if(bit_is_one == measurement){
            state_vector[i] /= norm_factor;
        }else{
            state_vector[i] = 0.0;
        }
    }

    // cout << "Measurement qubit " << qubit << " and got: " << measurement << "\n";
    addCircuit(qubit,"M");
    return measurement;
}


void QuantumCircuitBase::reset(int qubit){
    int measurement = measure_single_qubit(qubit);
    if(measurement == 1){
        X(qubit);
    }
}

void QuantumCircuitBase::resetAll(int index = 0){
    fill(state_vector.begin(), state_vector.end(), 0.0);
    state_vector[index] = 1.0;
}

string QuantumCircuitBase::measure_range_of_qubits(const vector<int> &qubits){

    size_t mask = 0;
    for(auto& q:qubits) mask |= 1<<q;
    map<size_t,double> prob;

    size_t num_states = 1<<qubit_count;

    for(size_t i=0; i<num_states; i++){
        prob[i&mask] += norm(state_vector[i]);
    }

    vector<size_t> outcomes;
    vector<double> weights;

    for(auto &a: prob){
        outcomes.push_back(a.first);
        weights.push_back(a.second);
    }

    static random_device rd;
    static mt19937 gen(rd());
    discrete_distribution<> dist(weights.begin(),weights.end());
    size_t index = dist(gen);
    double norm_factor = sqrt(weights[index]);
    size_t measurement = outcomes[index];

    for(int i=0; i<num_states; i++){
        if((i&mask) == measurement){
            state_vector[i] /= norm_factor;
        }else{
            state_vector[i] = 0.0;
        }
    }

    for(auto &q: qubits) circuit[q] += "[M]";
    string output;
    for(int q:qubits){
        output += (((measurement>>q) & 1) ? '1' : '0'); //Measurement returned in the same order as the qubits input vector
    }
    cout << "Measurement in order given: " << output;
    return output;
}

map<string,int> QuantumCircuitBase::run_range_of_qubits(int num_shots, const vector<int> &qubits){

    size_t mask = 0;
    for(auto& q:qubits) mask |= 1<<q;
    map<size_t,double> prob;

    size_t num_states = 1<<qubit_count;

    for(size_t i=0; i<num_states; i++){
        prob[i&mask] += norm(state_vector[i]);
    }

    vector<size_t> outcomes;
    vector<double> weights;

    for(auto &a: prob){
        outcomes.push_back(a.first);
        weights.push_back(a.second);
    }

    map<string,int> result;

    static random_device rd;
    static mt19937 gen(rd());
    discrete_distribution<> dist(weights.begin(),weights.end());

    for(int i=0;i<num_shots;i++){
        size_t index = dist(gen);
        double norm_factor = sqrt(weights[index]);
        size_t measurement = outcomes[index];

        string output;
        for(int q:qubits){
            output += (((measurement>>q) & 1) ? '1' : '0'); //Measurement returned in the same order as the qubits input vector
        }
        result[output]++;
    }
    for(auto &q: qubits) circuit[q] += "[M]";
    return result;
}


void QuantumCircuitBase::displayGraph() {
    QuantumVisualization::displayGraph(state_vector,qubit_count);
}

void QuantumCircuitBase::displayHeatMap() {
    QuantumVisualization::displayHeatMap(state_vector,qubit_count);
}

void QuantumCircuitBase::printProbabilities(){
    QuantumVisualization::printProbabilities(state_vector,qubit_count);
}

void QuantumCircuitBase::printState() {
    QuantumVisualization::printState(state_vector,qubit_count);
}

//Function for applying single qubit operations

void QuantumCircuitBase::applySingleQubitOp(int target_qubit, function<void(complex<double>&,complex<double>&)> op){
    if(target_qubit<0 || target_qubit>=qubit_count) throw out_of_range("Target qubit is out of range");

    size_t block_size = 1ULL<<target_qubit;
    size_t stride = 1ULL<<(target_qubit+1);

    for(size_t i=0;i<state_vector.size();i+=stride){
        for(size_t j=0;j<block_size;j++){
            op(state_vector[i+j],state_vector[i+j+block_size]);
        }
    }
}

//Function for two qubit operations

void QuantumCircuitBase::applyTwoQubitOp(int qubit_1, int qubit_2, function<void(complex<double>&,complex<double>&,complex<double>&,complex<double>&)> op){
    if(qubit_1 >= qubit_count || qubit_1 < 0 || qubit_2 >= qubit_count || qubit_2 <0) throw out_of_range("Qubits out of range.");
    if(qubit_1 == qubit_2) throw invalid_argument("Qubits cannot be the same");

    size_t q_b = max(qubit_1,qubit_2);
    size_t q_a = min(qubit_1,qubit_2);

    size_t outer_stride = 1 << (q_b+1);
    size_t inner_stride = 1 << (q_a+1);

    size_t bit_b = 1<<q_b;
    size_t bit_a = 1<<q_a;

    for(int i = 0; i<state_vector.size(); i+= outer_stride){
        for(int j = 0; j<bit_b; j+= inner_stride){
            for(int k = 0;k<bit_a; k++){
                int indbase = i+j+k;

                int ind00 = indbase;
                int ind01 = indbase + bit_a;
                int ind10 = indbase + bit_b;
                int ind11 = indbase + bit_a + bit_b;

                if(q_b == qubit_1) op(state_vector[ind00],state_vector[ind01],state_vector[ind10], state_vector[ind11]);
                else op(state_vector[ind00],state_vector[ind10],state_vector[ind01], state_vector[ind11]);
            }
        }
    }
}

//Funcion for applying controlled operations

void QuantumCircuitBase::applyControlledQubitOp(int control_qubit, int target_qubit, function<void(complex<double>&, complex<double>&)> op){
    if(control_qubit >= qubit_count || control_qubit < 0 || target_qubit >= qubit_count || target_qubit <0) throw out_of_range("Qubits out of range.");
    if(control_qubit == target_qubit) throw invalid_argument("Control and target qubits cannot be the same.");

    size_t control_mask = 1ULL << control_qubit;
    size_t block_size = 1ULL << target_qubit;
    size_t stride = 1ULL <<(target_qubit+1);

    for(size_t i=0;i<state_vector.size();i+=stride){
        for(size_t j=0;j<block_size;j++){
            if(((i+j)&control_mask)!=0) op(state_vector[i+j],state_vector[i+j+block_size]);
        }
    }
}

//Type 1: Pauli Gates

void QuantumCircuitBase::X(int target_qubit) {
    applySingleQubitOp(target_qubit,QuantumGates::X_Function());
    addCircuit(target_qubit, "X");
}

void QuantumCircuitBase::Y(int target_qubit){
    applySingleQubitOp(target_qubit,QuantumGates::Y_Function());
    addCircuit(target_qubit, "Y");
}

void QuantumCircuitBase::Z(int target_qubit){
    applySingleQubitOp(target_qubit,QuantumGates::Z_Function());
    addCircuit(target_qubit, "Z");
}

//Type 2: Superposition Gate

void QuantumCircuitBase::H(int target_qubit){
    applySingleQubitOp(target_qubit,QuantumGates::H_Function());
    addCircuit(target_qubit, "H");
}

//Type 3: Phase Gate 

void QuantumCircuitBase::S(int target_qubit){
    applySingleQubitOp(target_qubit,QuantumGates::Phase_Function(QuantumGates::I));
    addCircuit(target_qubit, "S");
}

void QuantumCircuitBase::Sdg(int target_qubit){
    applySingleQubitOp(target_qubit,QuantumGates::Phase_Function(-1.0 * QuantumGates::I));
    addCircuit(target_qubit, "S");
}

void QuantumCircuitBase::T(int target_qubit) {
    applySingleQubitOp(target_qubit,QuantumGates::Phase_Function(polar(1.0, M_PI / 4.0)));
    addCircuit(target_qubit, "T");
}

void QuantumCircuitBase::Tdg(int target_qubit) {
    applySingleQubitOp(target_qubit,QuantumGates::Phase_Function(polar(1.0, -M_PI / 4.0)));
    addCircuit(target_qubit, "Tdg");
}

void QuantumCircuitBase::P(int target_qubit, const double theta){
    applySingleQubitOp(target_qubit,QuantumGates::Phase_Function(polar(1.0,theta)));
    addCircuit(target_qubit, "P");
}

void QuantumCircuitBase::Rz(int target_qubit, const double theta){
    applySingleQubitOp(target_qubit,QuantumGates::Rz_Function(theta));
    addCircuit(target_qubit,"Rz("+to_string(theta)+")");
}

void QuantumCircuitBase::Rx(int target_qubit, const double theta){
    applySingleQubitOp(target_qubit,QuantumGates::Rx_Function(theta));
    addCircuit(target_qubit,"Rx("+to_string(theta)+")");
}

void QuantumCircuitBase::Ry(int target_qubit, const double theta){
    applySingleQubitOp(target_qubit,QuantumGates::Ry_Function(theta));
    addCircuit(target_qubit,"Ry("+to_string(theta)+")");
}

//Type 4: Entangling gate

void QuantumCircuitBase::CX(int control_qubit, int target_qubit){
    applyControlledQubitOp(control_qubit,target_qubit, QuantumGates::X_Function());
    addCircuit(control_qubit, "C", target_qubit, "X");
}

void QuantumCircuitBase::CY(int control_qubit, int target_qubit){
    applyControlledQubitOp(control_qubit,target_qubit, QuantumGates::Y_Function());
    addCircuit(control_qubit, "C", target_qubit, "Y");
}

void QuantumCircuitBase::CZ(int control_qubit, int target_qubit){
    applyControlledQubitOp(control_qubit,target_qubit, QuantumGates::Z_Function());
    addCircuit(control_qubit, "C", target_qubit, "Z");
}

void QuantumCircuitBase::CH(int control_qubit, int target_qubit){
    applyControlledQubitOp(control_qubit,target_qubit, QuantumGates::H_Function());
    addCircuit(control_qubit, "C", target_qubit, "H");
}

void QuantumCircuitBase::CS(int control_qubit, int target_qubit){
    applyControlledQubitOp(control_qubit,target_qubit, QuantumGates::Phase_Function(QuantumGates::I));
    addCircuit(control_qubit, "C", target_qubit, "S");
}

void QuantumCircuitBase::CSdg(int control_qubit, int target_qubit){
    applyControlledQubitOp(control_qubit,target_qubit, QuantumGates::Phase_Function(-1.0*QuantumGates::I));
    addCircuit(control_qubit, "C", target_qubit, "Sdg");
}

void QuantumCircuitBase::CT(int control_qubit, int target_qubit){
    applyControlledQubitOp(control_qubit,target_qubit, QuantumGates::Phase_Function(polar(1.0,M_PI/4.0)));
    addCircuit(control_qubit, "C", target_qubit, "T");
}

void QuantumCircuitBase::CTdg(int control_qubit, int target_qubit){
    applyControlledQubitOp(control_qubit,target_qubit, QuantumGates::Phase_Function(polar(1.0,-M_PI/4.0)));
    addCircuit(control_qubit, "C", target_qubit, "Tdg");
}

void QuantumCircuitBase::CP(int control_qubit, int target_qubit,const double theta){
    applyControlledQubitOp(control_qubit,target_qubit, QuantumGates::Phase_Function(polar(1.0,theta)));
    addCircuit(control_qubit, "C", target_qubit, "P("+to_string(theta)+")");
}

void QuantumCircuitBase::CRz(int control_qubit, int target_qubit, const double theta){
    applyControlledQubitOp(control_qubit,target_qubit, QuantumGates::Rz_Function(theta));
    addCircuit(control_qubit, "C", target_qubit, "Rz("+to_string(theta)+")");
}

void QuantumCircuitBase::CRx(int control_qubit, int target_qubit, const double theta){
    applyControlledQubitOp(control_qubit,target_qubit, QuantumGates::Rx_Function(theta));
    addCircuit(control_qubit, "C", target_qubit, "Rx("+to_string(theta)+")");
}

void QuantumCircuitBase::CRy(int control_qubit, int target_qubit, const double theta){
    applyControlledQubitOp(control_qubit,target_qubit, QuantumGates::Ry_Function(theta));
    addCircuit(control_qubit, "C", target_qubit, "Ry("+to_string(theta)+")");
}

void QuantumCircuitBase::SWAP(int qubit_1, int qubit_2){
    applyTwoQubitOp(qubit_1, qubit_2, QuantumGates::SWAP_Function());
}

void QuantumCircuitBase::iSWAP(int qubit_1, int qubit_2){
    applyTwoQubitOp(qubit_1, qubit_2, QuantumGates::iSWAP_Function());
}

