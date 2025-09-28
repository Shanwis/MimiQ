#include <iostream>
#include <cmath>
#include <iomanip>
#include <stdexcept>
#include "../include/QuantumCircuit.h"
using namespace std;

// Constructor with member initializer list
QuantumCircuit::QuantumCircuit(int n) :
    qubit_count(n),
    NOT_MATRIX({ {0.0, 1.0}, {1.0, 0.0} }),
    HADAMARD_MATRIX({ {1.0 / sqrt(2), 1.0 / sqrt(2)}, {1.0 / sqrt(2), -1.0 / sqrt(2)} }),
    Z_MATRIX({ {1.0, 0.0}, {0.0, -1.0} })
{
    vector<vector<double>> initial_qubit(2, vector<double>(1, 0.0));
    initial_qubit[0][0] = 1.0; // Initialize each qubit to the |0> state

    for (int i = 0; i < n; i++) {
        mp[i] = initial_qubit;
    }
}

// Gate implementations
void QuantumCircuit::X(int target_qubit) {
    mp[target_qubit] = matrixMultiply(NOT_MATRIX, mp[target_qubit]);
}

void QuantumCircuit::H(int target_qubit) {
    mp[target_qubit] = matrixMultiply(HADAMARD_MATRIX, mp[target_qubit]);
}

void QuantumCircuit::Z(int target_qubit) {
    mp[target_qubit] = matrixMultiply(Z_MATRIX, mp[target_qubit]);
}

void QuantumCircuit::measure() {
    if (qubit_count != 2) {
        cerr << "Error: Measurement is currently implemented for 2-qubit systems only." << endl;
        return;
    }
    
    vector<vector<double>> combined_state = tensorProduct(mp[0], mp[1]);
    vector<string> basis_states = {"00", "01", "10", "11"};

    cout << fixed << setprecision(6);
    cout << "--- Measurement Results ---" << endl;
    for (size_t i = 0; i < combined_state.size(); i++) {
        double amplitude = combined_state[i][0];
        double probability = amplitude * amplitude;
        cout << "Probability of |" << basis_states[i] << ">: " << probability << endl;
    }
    cout << "-------------------------" << endl;
}

// Private helper function implementations
vector<vector<double>> QuantumCircuit::matrixMultiply(const vector<vector<double>>& A, const vector<vector<double>>& B) {
    int m = A.size();
    int n = A[0].size();
    int n2 = B.size();
    int p = B[0].size();

    if (n != n2) {
        throw invalid_argument("Matrix dimensions are not compatible for multiplication.");
    }

    vector<vector<double>> result(m, vector<double>(p, 0.0));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            for (int k = 0; k < n; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return result;
}

vector<vector<double>> QuantumCircuit::tensorProduct(const vector<vector<double>>& A, const vector<vector<double>>& B) {
    int m = A.size();
    int n = B.size();
    vector<vector<double>> result(m * n, vector<double>(1, 0.0));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            result[i * n + j][0] = A[i][0] * B[j][0];
        }
    }
    return result;
}