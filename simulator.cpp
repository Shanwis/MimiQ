#include <bits/stdc++.h>
using namespace std;

class QuantumCircuit {
private:
    int qubit_count = 1;
    map<int, vector<vector<double>>> mp;

    vector<vector<double>> NOT_MATRIX = {
        {0.0, 1.0},
        {1.0, 0.0}
    };

    vector<vector<double>> HADAMARD_MATRIX = {
        {1.0 / sqrt(2),  1.0 / sqrt(2)},
        {1.0 / sqrt(2), -1.0 / sqrt(2)}
    };

    vector<vector<double>> matrixMultiply(const vector<vector<double>>& A,
                                          const vector<vector<double>>& B) {
        int m = A.size();        // rows in A
        int n = A[0].size();     // cols in A
        int n2 = B.size();       // rows in B
        int p = B[0].size();     // cols in B

        if (n != n2) {
            throw invalid_argument("Number of columns of A must equal number of rows of B");
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

public:
    QuantumCircuit() {
        vector<vector<double>> qubit(2, vector<double>(1, 0.0));
        qubit[0][0] = 1.0; // |0> state
        mp[0] = qubit;
    }

    void X() {
        mp[0] = matrixMultiply(NOT_MATRIX, mp[0]);
    }

    void H() {
        mp[0] = matrixMultiply(HADAMARD_MATRIX, mp[0]);
    }

    void measure() {
    double p0 = mp[0][0][0] * mp[0][0][0]; // |alpha|^2
    double p1 = mp[0][1][0] * mp[0][1][0]; // |beta|^2

    cout << fixed << setprecision(6);
    cout << "Probability of |0>: " << p0 << endl;
    cout << "Probability of |1>: " << p1 << endl;
}
};

int main() {
    QuantumCircuit qc;
    // qc.X();
    qc.H();
    qc.H();   // Apply Hadamard
    qc.measure();
    return 0;
}
