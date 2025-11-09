#include<iostream>
#include<vector>
#include "../include/QuantumCircuitParallel.h"
#include<random>
#include<omp.h>
using namespace std;
#define NUM_EPOCHS 200



template<class T>
void printVector(vector<T> &a){
    cout<<endl;
    for(auto i: a){
        cout<<i<<','<<" ";
    }
    cout<<endl;
}
double encoder(double x){
    return x*3.14;
}
double singleStep(QuantumCircuitParallel &qc,
    vector<double> & params,
    double x)
{
    double e = 0;
    qc.H(0);
    qc.Rx(0,encoder(x));
    qc.CRx(0,1,params[0]);
    qc.CRy(0,1,params[1]);
    qc.CRz(0,1,params[2]);
    qc.CRx(1,0,params[3]);
    qc.CRy(1,0,params[4]);
    qc.CRz(1,0,params[5]);
    vector<int> args = {1};
    qc.measure_single_qubit(0);
    e = qc.expectZ(args);
    return e;
}

double paramShift(QuantumCircuitParallel &qc,
                  vector<double> &params,
                  double x,
                  double y,
                  double lr = 0.1)
{
    double e = singleStep(qc, params, x);

    const double shift = M_PI / 2.0;
    vector<double> grads(params.size());

    for (size_t i = 0; i < params.size(); ++i) {
        double old = params[i];

        params[i] = old + shift;
        double e_plus = singleStep(qc, params, x);

        params[i] = old - shift;
        double e_minus = singleStep(qc, params, x);

        // restore
        params[i] = old;

        // ∂e/∂θ_i
        double de_dtheta = 0.5 * (e_plus - e_minus);
        grads[i] = de_dtheta;
    }

    double err = (y - e);
    for (size_t i = 0; i < params.size(); ++i) {
        params[i] += lr * err * (2.0 * grads[i]);
    }

    return err * err;
}




int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    const int n = 100;
    std::vector<double> values(n);
    for (int i=0; i<n;i++){
        values[i] = i/double(n);
    }

    QuantumCircuitParallel qc(2);

    vector<double> params(6);
    for (auto &p : params) p = dist(gen);

    for (int epoch = 0; epoch < NUM_EPOCHS; ++epoch) {
        double epoch_loss = 0.0;

        for (int j = 0; j < (int)values.size() - 1; ++j) {
            epoch_loss += paramShift(qc, params, values[j], values[j + 1], 0.01);
        }

        double rmse = std::sqrt(epoch_loss) / (values.size() - 1);
        printf("\nEPOCH %d  RMSE: %.6f\n", epoch, rmse);
        printVector(params);
    }
    return 0;
}
