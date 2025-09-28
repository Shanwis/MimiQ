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

    vector<vector<double>> Z_MATRIX = {
        {1.0,0.0},
        {0.0,-1.0}
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

    vector<vector<double>> tensorProduct(const vector<vector<double>>& A,
                                     const vector<vector<double>>& B) {
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

public:
    QuantumCircuit(int n) {
        vector<vector<double>> qubit(2, vector<double>(1, 0.0));
        qubit[0][0] = 1.0; // |0> state
        for(int i=0;i<n;i++) {
           mp[i] = qubit;
        }
    }

    void X(int n) {
        mp[n] = matrixMultiply(NOT_MATRIX, mp[n]);
    }

    void H(int n) {
        mp[n] = matrixMultiply(HADAMARD_MATRIX, mp[n]);
    }

    void Z(int n) {
        mp[n] = matrixMultiply(Z_MATRIX, mp[n]);
    }

    void measure() {
        vector<vector<double>> combined = tensorProduct(mp[0], mp[1]);

        vector<string> basis = {"00", "01", "10", "11"};

        cout << fixed << setprecision(6);
        for (int i = 0; i < combined.size(); i++) {
            double prob = combined[i][0] * combined[i][0]; // |amplitude|^2
            cout << "Probability of |" << basis[i] << ">: " << prob << endl;
        }
    }
};

int main() {
    QuantumCircuit qc(2);
    qc.X(1);
    qc.H(0);
    qc.measure();
    return 0;
}

