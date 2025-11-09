#ifndef QUANTUMVISUALIZATION_H
#define QUANTUMVISUALIZATION_H

#include <string>
#include <complex>
#include <vector>

namespace QuantumVisualization{

    constexpr double PROB_THRESHOLD = 0.01;
    //Helper to generate the states
    std::vector<std::string> generateBasisStates(int n);

    void printState(const std::vector<std::complex<double>>& state_vector, int qubit_count);
    void printCircuit(std::vector<std::string> circuit, int qubit_count);
    void printProbabilities(const std::vector<std::complex<double>>& state_vector, int qubit_count);

    void displayGraph(const std::vector<std::complex<double>>& state_vector, int qubit_count);
    void displayHeatMap(const std::vector<std::complex<double>>& state_vector, int qubit_count);
}


#endif