#ifndef QUANTUMCIRCUIT_H
#define QUANTUMCIRCUIT_H

#include<vector>
#include<complex>
#include<string>

using namespace std;

class QuantumCircuit {
private:
    //member var
    int qubit_count;
    vector<complex<double>> state_vector;

    //Helper function to generate labels like "00", "01", etc
    vector<string> generateBasisStates(int n);

    //Circuit
    vector<string> circuit;

public:
    //Constructor
    QuantumCircuit(int n);

    //Public gate methods
    void H(int target_qubit);
    void X(int target_qubit);
    void Z(int target_qubit);
    void Y(int target_qubit);
    void S(int target_qubit);
    void CNOT(int control_qubit, int target_qubit);
    void addCircuit(int qubit, char gate, int t_qubit);

    //measurement
    void collapse();

    // Helper to print the current state vector
    void printState();
    void printCircuit();
    void measureProbabilities();

    void displayGraph();
    void HeatMapRep();
};

#endif