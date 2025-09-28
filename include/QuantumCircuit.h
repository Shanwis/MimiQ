#ifndef QUANTUMCIRCUIT_H
#define QUANTUMCIRCUIT_H

#include<vector>
#include<map>
#include<string>

using namespace std;

class QuantumCircuit {
private:
    //member var
    int qubit_count;
    map<int, vector<vector<double>>> mp;

    //Gate matrix
    vector<vector<double>> NOT_MATRIX;
    vector<vector<double>> HADAMARD_MATRIX;
    vector<vector<double>> Z_MATRIX;

    //Private helper functions
    vector<vector<double>> matrixMultiply(const vector<vector<double>>& A, const vector<vector<double>>& B);
    vector<vector<double>> tensorProduct(const vector<vector<double>>& A, const vector<vector<double>>& B);

public:
    //Constructor
    QuantumCircuit(int n);

    //Public gate methods
    void H(int n);
    void X(int n);
    void Z(int n);

    //measurement
    void measure();
};

#endif