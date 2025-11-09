#ifndef QUANTUMVISUALIZATION_H
#define QUANTUMVISUALIZATION_H

#include <string>
#include <complex>
#include <vector>

namespace QuantumVisualization{

    constexpr double PROB_THRESHOLD = 0.01;
    //Helper to generate the states
    std::vector<std::string> generateBasisStates(int n);
    //Prints the current states of the circuit
    void printState(const std::vector<std::complex<double>>& state_vector, int qubit_count);
    //prints the circuits
    void printCircuit(const std::vector<std::string>& circuit, int qubit_count);
    //prints probabilities
    void printProbabilities(const std::vector<std::complex<double>>& state_vector, int qubit_count);
    //displays probabilities as graph and heat map 
    void displayGraph(const std::vector<std::complex<double>>& state_vector, int qubit_count);
    void displayHeatMap(const std::vector<std::complex<double>>& state_vector, int qubit_count);
}


#endif