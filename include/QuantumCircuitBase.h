#ifndef QUANTUMCIRCUITBASE_H
#define QUANTUMCIRCUITBASE_H

#include<vector>
#include<map>
#include<complex>
#include<string>
#include<functional>

using namespace std;

class QuantumCircuitBase {
protected:
    //member var
    int qubit_count;
    vector<complex<double>> state_vector;

    //Circuit
    vector<string> circuit;

    //Add to the ASCII representation
    void addCircuit(int qubit,const string &gate);
    void addCircuit(int qubit1,const string &gate1, int qubit2, const string &gate2);
    void alignCircuitColumns();

    //For performing operations
    virtual void applySingleQubitOp(int target_qubit, function<void(complex<double>&,complex<double>&)> op);
    virtual void applyControlledQubitOp(int control_qubit, int target_qubit, function<void(complex<double>&, complex<double>&)> op);

public:
    //Constructor
    QuantumCircuitBase(int n);
    virtual ~QuantumCircuitBase() = default;

    //Public gate methods
    virtual void H(int target_qubit);
    virtual void X(int target_qubit);
    virtual void Z(int target_qubit);
    virtual void Y(int target_qubit);
    virtual void S(int target_qubit);
    virtual void Sdg(int target_qubit);
    virtual void T(int target_qubit);
    virtual void Tdg(int target_qubit);
    virtual void P(int target_qubit, const double theta);
    virtual void Rz(int target_qubit, const double theta);
    virtual void Rx(int target_qubit, const double theta);
    virtual void Ry(int target_qubit, const double theta);
    virtual void CX(int control_qubit, int target_qubit);
    virtual void CZ(int control_qubit, int target_qubit);
    virtual void CH(int control_qubit, int target_qubit);
    virtual void CY(int control_qubit, int target_qubit);
    virtual void CS(int control_qubit, int target_qubit);
    virtual void CSdg(int control_qubit, int target_qubit);
    virtual void CT(int control_qubit, int target_qubit);
    virtual void CTdg(int control_qubit, int target_qubit);
    virtual void CP(int control_qubit, int target_qubit, const double theta);
    virtual void CRx(int control_qubit, int target_qubit, const double theta);
    virtual void CRy(int control_qubit, int target_qubit, const double theta);
    virtual void CRz(int control_qubit, int target_qubit, const double theta);
    
    //measurement
    string collapse();
    map<string,int> run(int num_shots);
    int measure_single_qubit(int qubit);
    string measure_range_of_qubits(const vector<int> &qubits);
    map<string,int> run_range_of_qubits(int num_shots, const vector<int> &qubits);
    void reset(int qubit);

    // Helper to print the current state vector
    std::complex<double> getProbAmplitude(const std::vector<std::complex<double>>& state_vector, int qubit){
        return state_vector[qubit];
    };

    void printState();
    void printCircuit();
    void printProbabilities();

    void displayGraph();
    void displayHeatMap();
};

#endif