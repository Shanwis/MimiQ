#include <MaQrel/QuantumCircuitParallel.h>
#include<iostream>
#include<string>
#include<stdexcept>

int main(){
    QuantumCircuitParallel qc(2);

    qc.H(0);
    qc.CX(0,1);

    std::cout << "Enter the 2 bit string to be sent: ";
    std::string value_to_be_sent;
    std::cin >> value_to_be_sent;

    if(value_to_be_sent == "01"){
        qc.X(0);
    }else if(value_to_be_sent == "10"){
        qc.Z(0);
    }else if(value_to_be_sent == "11"){
        qc.Y(0);
    }else if(value_to_be_sent == "00"){
        
    }else{
        throw std::invalid_argument("Invalid string inputted");
        return 1;
    }

    qc.CX(0,1);
    qc.H(0);

    qc.printCircuit();
    qc.printProbabilities();
    qc.displayGraph();
}