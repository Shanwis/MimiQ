#include <iostream>
#include <algorithm>
#include <random>
#include <cmath>
#include <iomanip>
#include <stdexcept>
#include <fstream> // For std::ofstream
#include <cstdlib> // For system()
#include <omp.h>
#include "../include/QuantumCircuit.h"
using namespace std;

// Constructor with member initializer list
QuantumCircuit::QuantumCircuit(int n) :
    qubit_count(n)
{
    if(n<=0) {
        throw invalid_argument("Number of qubits must be positive.");
    }

    size_t state_size = 1<<n; //size is 2^n
    state_vector.resize(state_size,0);
    circuit.resize(qubit_count, "");

    state_vector[0] = 1.0; //Initialize the system to first state.
}

void QuantumCircuit::addCircuit(int qubit, char gate, int t_qubit = -1){
    for(int i=0; i<qubit_count; i++){
        if(i == qubit){ circuit[i] += "-[" + string(1,gate) + "]-"; }
        else if (i==t_qubit){ circuit[i] += "-[*]-"; }
        else if (i>qubit && i<t_qubit) { circuit[i] += "--+--";}
        else circuit[i] += "-----";
    }
}

void QuantumCircuit::printCircuit(){
    for(int i=0; i<qubit_count; i++){
        cout << i << " " << circuit[i] << "\n";
    }
}

void QuantumCircuit::X(int target_qubit) {
    if(target_qubit >= qubit_count || target_qubit < 0) throw out_of_range("Qubit index out of range.");

    size_t stride = 1 << (target_qubit + 1);
    size_t block_size = 1 << target_qubit;

    #pragma omp parallel for
    for(size_t i=0; i<state_vector.size(); i+=stride){
        for(size_t j=0; j<block_size; j++){
            swap(state_vector[i+j], state_vector[i+j+block_size]); //X gate swaps amplitudes
        }
    }
    addCircuit(target_qubit, 'X');
}

void QuantumCircuit::H(int target_qubit){
    if(target_qubit >= qubit_count || target_qubit <0) throw out_of_range("Qubits index out of range.");

    const complex<double> inv_sq_2 = 1.0 / sqrt(2.0);
    size_t stride = 1 << (target_qubit + 1);
    size_t block_size = 1 << target_qubit;

    #pragma omp parallel for
    for(size_t i = 0; i<state_vector.size(); i+=stride){
        for(size_t j=0; j<block_size; ++j) {

            //applying Hadamard on relevent pairs
            complex<double> a = state_vector[i+j];
            complex<double> b = state_vector[i+j+block_size];
            state_vector[i+j] = inv_sq_2 * (a+b);
            state_vector[i+j+block_size] = inv_sq_2 * (a-b);
        }
    }
    addCircuit(target_qubit, 'H');
}

void QuantumCircuit::Z(int target_qubit){
    if(target_qubit >= qubit_count || target_qubit <0) throw out_of_range("Qubits index out of range.");

    size_t mask = 1<< target_qubit;
    
    #pragma omp parallel for shared(mask)
    for(size_t i = 0; i < state_vector.size(); ++i){
        if((i&mask) != 0){
            //apply -1 phase
            state_vector[i] *= -1.0;
        }
    }
    addCircuit(target_qubit, 'Z');
}

const complex<double> I(0.0,1.0);

void QuantumCircuit::Y(int target_qubit){
    if(target_qubit >= qubit_count || target_qubit <0) throw out_of_range("Qubits index out of range.");
    
    size_t stride = 1 << (target_qubit + 1);
    size_t block_size = 1 << target_qubit;

    #pragma omp parallel for
    for(size_t i = 0; i<state_vector.size(); i+=stride){
        for(size_t j=0; j<block_size; ++j) {

            //applying Y on relevent pairs
            complex<double> a = state_vector[i+j];
            complex<double> b = state_vector[i+j+block_size];
            state_vector[i+j] = -I * b;
            state_vector[i+j+block_size] = I * a;
        }
    }
    addCircuit(target_qubit, 'Y');
}


void QuantumCircuit::S(int target_qubit){
    if(target_qubit >= qubit_count || target_qubit <0) throw out_of_range("Qubits index out of range.");
    size_t mask = 1<< target_qubit;
    #pragma omp parallel for shared(mask)
    for(size_t i = 0; i < state_vector.size(); ++i){
        if((i&mask) != 0){
            //apply phase of I
            state_vector[i] *= I;
        }
    }
    addCircuit(target_qubit, 'S');
}

void QuantumCircuit::CNOT(int control_qubit, int target_qubit){
    if(control_qubit >= qubit_count || control_qubit < 0 || target_qubit >= qubit_count || target_qubit <0 || control_qubit == target_qubit) throw out_of_range("Qubits out of range.");

    size_t control_mask = 1 << control_qubit;
    size_t target_mask = 1 << target_qubit;

    #pragma omp parallel for
    for(size_t i=0; i<state_vector.size(); ++i) {
        if((i&control_mask) != 0){
            if(i < (i^target_mask)) swap(state_vector[i], state_vector[i^target_mask]);
        }
    }
    addCircuit(control_qubit, 'C', target_qubit);
}

vector<string> QuantumCircuit::generateBasisStates(int n){
    vector<string> basis_states;
    size_t num_states = 1<<n;
    for(size_t i = 0; i<num_states; i++){
        string basis = "";
        int temp = i;
        for(int j=0; j<n; j++){
            basis += to_string(temp%2);
            temp/=2;
        }
        reverse(basis.begin(), basis.end());
        basis_states.push_back(basis);
    }
    return basis_states;
}

void QuantumCircuit::collapse(){
    vector<string> basis_states = generateBasisStates(qubit_count);
    vector<double> weights;
    for(auto &a:state_vector){
        weights.push_back(norm(a));
    }

    //Getting the state which it would be at
    static random_device rd;
    static mt19937 gen(rd());
    discrete_distribution<> dist(weights.begin(), weights.end());
    int index = dist(gen);
    
    fill(state_vector.begin(), state_vector.end(), 0.0);
    state_vector[index] = 1.0;
    cout << basis_states[index] << "\n";
}

// Display graph of probabilities
void QuantumCircuit::displayGraph() {
    // Step 1: Write data to a temporary file
     const double prob_threshold = 0.01;

    ofstream dataFile("prob_data.dat");
    if (!dataFile.is_open()) {
        cerr << "Error: Could not open data file for gnuplot." << endl;
        return;
    }

    vector<string> basis_states = generateBasisStates(qubit_count);
    for (size_t i = 0; i < state_vector.size(); ++i) {
        // Gnuplot format: "Label" Value
        double prob = norm(state_vector[i]);
        if(prob>=prob_threshold){
            dataFile << "\"" << basis_states[i] << "\" " << prob << "\n";
        }
    }
    dataFile.close();

    #ifdef _WIN32
        string terminal = "set terminal qt size 800,600 font 'Verdana,10'; ";
    #else
        string terminal = "set terminal x11 size 800,600 font 'Verdana,10'; ";
    #endif


    // Step 2 & 3: Create a gnuplot command and execute it
    string gnuplot_command = 
        "gnuplot -e \""
        + terminal +  // Use a modern terminal
        "set title 'Quantum State Probabilities'; "
        "set ylabel 'Probability'; "
        "set xlabel 'Basis States'; "
        "set yrange [0:1.1]; " // Set y-axis range from 0 to 1.1
        "set style fill solid 0.5; " // Fill bars with a solid color
        "set boxwidth 0.5; " // Set the width of the bars
        "set xtics rotate by -45; " // Rotate x-axis labels for readability
        "plot 'prob_data.dat' using 2:xtic(1) with boxes notitle, '' using (\\$0):(\\$2+0.05):(sprintf('%.3f',\\$2)) with labels font ',10' notitle; " // The main plot command
        "pause -1 'Press Enter to continue...'; " // Keep the window open
        "\"";

    cout << "Displaying graph with gnuplot..." << endl;
    system(gnuplot_command.c_str());

    // Optional: Clean up the temporary file
    remove("prob_data.dat");
}

// Display heatmap of of probabilities
void QuantumCircuit::HeatMapRep() {
    ofstream dataFile("prob_data.dat");

    int grid_size = 1 << (qubit_count/2);

    if (!dataFile.is_open()) {
        cerr << "Error: Could not open data file for gnuplot." << endl;
        return;
    }

    for(int row=0; row<grid_size; ++row){
        for(int col=0; col < grid_size; ++col){
            size_t index = row * grid_size + col;
            dataFile << norm(state_vector[index]) << " ";
        }
        dataFile << "\n"; 
    }

    dataFile.close();

    #ifdef _WIN32
        string terminal = "set terminal qt size 800,600 font 'Verdana,10'; ";
    #else
        string terminal = "set terminal x11 size 800,600 font 'Verdana,10'; ";
    #endif


    // Step 2 & 3: Create a gnuplot command and execute it
    string gnuplot_command = 
        "gnuplot -e \""
        + terminal +  // Use a modern terminal
        "set title 'Quantum State Probability heatmap';"
        "unset xlabel; unset ylabel; unset xtics; unset ytics; "
        "set palette defined (0 'black', 0.1 'dark-blue', 0.5 'yellow', 1 'white'); " // A nice color palette
        "set cblabel 'Probability';"
        "set cbrange [0:1];"
        "plot 'prob_data.dat' matrix with image notitle; " // The main plot command
        "pause -1 'Press Enter to continue...'; " // Keep the window open
        "\"";

    cout << "Displaying graph with gnuplot..." << endl;
    system(gnuplot_command.c_str());

    // Optional: Clean up the temporary file
    //remove("prob_data.dat");
}

void QuantumCircuit::measureProbabilities(){
    vector<string> basis_states = generateBasisStates(qubit_count);

    cout << fixed << setprecision(6);
    cout << qubit_count << "-Qubit Measurement Results" << "\n";
    for(size_t i = 0; i<state_vector.size(); ++i) {
        double prob = norm(state_vector[i]);
        cout << "Probability of |" << basis_states[i] << ">: " << prob << "\n";
    }
    cout << "----------------------------\n";
    // displayGraph();
}

void QuantumCircuit::printState() {
    cout << "Current State Vector" << "\n";
    vector<string>basis_states = generateBasisStates(qubit_count);
    for(size_t i=0; i<state_vector.size(); i++){
        cout << "|" << basis_states[i] << "> :" << state_vector[i] << "\n";
    }
}


