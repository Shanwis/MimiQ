#ifndef QUANTUMCIRCUITBASE_H
#define QUANTUMCIRCUITBASE_H

#include<vector>
#include<complex>
#include<string>
#include<functional>

using namespace std;

class QuantumCircuitBase {
protected:
    //member var
    int qubit_count;
    vector<complex<double>> state_vector;

    //Helper function to generate labels like "00", "01", etc
    vector<string> generateBasisStates(int n);

    //Circuit
    vector<string> circuit;

public:
    //Constructor
    QuantumCircuitBase(int n);
    virtual ~QuantumCircuitBase() = default;

    //Public gate methods
    virtual void H(int target_qubit) = 0;
    virtual void X(int target_qubit) = 0;
    virtual void Z(int target_qubit) = 0;
    virtual void Y(int target_qubit) = 0;
    virtual void S(int target_qubit) = 0;
    virtual void T(int target_qubit) = 0;
    virtual void Tdg(int target_qubit) = 0;
    virtual void Rz(int target_qubit, const double theta) = 0;
    virtual void CX(int control_qubit, int target_qubit) = 0;
    virtual void CZ(int control_qubit, int target_qubit) = 0;
    virtual void CH(int control_qubit, int target_qubit) = 0;
    virtual void CY(int control_qubit, int target_qubit) = 0;
    void addCircuit(int qubit, char gate, int t_qubit);
    
    //measurement
    string collapse();
    int measure_single_qubit(int qubit);

    // Helper to print the current state vector
    void printState();
    void printCircuit();
    void printProbabilities();

    void displayGraph();
    void displayHeatMap();
};

#endif