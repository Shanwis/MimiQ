#include<iostream>
#include<vector>
#include "../include/QuantumCircuitParallel.h"
#include<random>
#include<omp.h>
using namespace std;
#define NUM_EPOCHS 10



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
    e = qc.expectZ(args);
    qc.measure_single_qubit(1);
    return e;
}

double paramShift(QuantumCircuitParallel &qc,
    vector<double> &params,
    double x,
    double y,
    double lr = .1)
{
    double e1,e2,e;
    double shift = M_PI/2;
    double error = 0;
    e = singleStep(qc,params,x);
    for(int i =0;i<params.size();i++){
        params[i] +=shift;
        e1 = singleStep(qc,params,x);
        params[i] -=2*shift;
        e2 = singleStep(qc,params,x);
        params[i] += lr*.5*(e1-e2);
        error += (y-e)*(y-e);
    }
    return error;
}



int main(){
    std::random_device rd;  // Non-deterministic seed
    QuantumCircuitParallel qc(2);
    qc.H(1);
    int n = 8;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);
    // std::normal_distribution<double> ndist(0,1);
    std::vector<double> values(n);
    vector<double> params(6);
    for(auto & v: params){
        v = dist(gen);
    }

    for (auto& v : values) {
        v = dist(gen);
    }

    //train loop
    double error;
    for(int i=0;i<NUM_EPOCHS;i++){
        printf("\nEPOCH %d:\n",i);

        for(int j=0;j<values.size()-1;j++){
            int e = paramShift(qc,params,values[j],values[j+1]);
        }
        error=sqrt(error)/((values.size()-1)*6);
        printf("MAE: %d",error);
        printVector(params);
    }
    return 0;
}