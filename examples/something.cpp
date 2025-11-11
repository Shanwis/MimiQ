#include "../include/VQA.h"
#include<iostream>
using namespace std;
int main(){
    QNN<double> qnn(1);
    qnn.H(0);
    qnn.Rx(0);
    qnn.printCircuit();
    for(int i =0;i<100;i++){
        for(int j=0;j<10;j++){
            double err = qnn.backward(j/9,((j+1)%10)/9,.1);
            cout<<err<<endl;
        }
    }
    return 0;
}