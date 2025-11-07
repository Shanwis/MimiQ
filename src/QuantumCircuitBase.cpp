#include <iostream>
#include <algorithm>
#include <random>
#include <cmath>
#include <iomanip>
#include <stdexcept>
#include <fstream> // For std::ofstream
#include <cstdlib> // For system()
#include "../include/QuantumCircuitBase.h"
using namespace std;

const double prob_threshold = 0.01;

// Constructor with member initializer list
QuantumCircuitBase::QuantumCircuitBase(int n) :
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

void QuantumCircuitBase::addCircuit(int qubit, const string &gate){
    string box_name = "["+gate+"]";
    int gate_width = box_name.length();

    // size_t max_length = 0;
    // for(int i=0;i<qubit_count;i++){
    //     max_length = max(max_length,circuit[i].length());
    // }

    for(int i=0;i<qubit_count;i++){
        // int padding = max_length-circuit[i].length();
        // circuit[i] += string(padding,'-');

        if(i==qubit) circuit[i]+=box_name;
        else circuit[i] += string(gate_width,'-');
    }
}

void QuantumCircuitBase::addCircuit(int qubit1,const string &gate1, int qubit2,const string &gate2){

    int max_gate_width = max(gate1.length(),gate2.length());
    string seperator = "-+" + string(max_gate_width+1,'-');

    string box_name_1 = "["+gate1+string(max_gate_width-gate1.length(),' ')+"]";
    string box_name_2 = "["+gate2+string(max_gate_width-gate2.length(),' ')+"]";

    // size_t max_length = 0;
    // for(int i=0;i<qubit_count;i++){
    //     max_length = max(max_length,circuit[i].length());
    // }

    for(int i=0;i<qubit_count;i++){
        // int padding = max_length-circuit[i].length();
        // circuit[i] += string(padding,'-');

        if(i==qubit1) circuit[i]+=box_name_1;
        else if(i>qubit1 && i<qubit2) circuit[i]+=seperator;
        else if(i==qubit2) circuit[i]+=box_name_2;
        else circuit[i]+= string(max_gate_width+2,'-');
    }
}

void QuantumCircuitBase::printCircuit(){
    cout << "--- Circuit Diagram ---\n";
    for(int i=0; i<qubit_count; i++){
        cout << 'q' << i << " " << circuit[i] << "\n";
    }
    cout << "-----------------------\n";
}

//collapse
string QuantumCircuitBase::collapse(){
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
    for(int i=0; i<qubit_count; i++){
       circuit[i] += "[M]";
    }
    return basis_states[index];
}

int QuantumCircuitBase::measure_single_qubit(int qubit){
    
    double prob_of_one = 0.0;
    size_t num_states = 1<<qubit_count;

    for(int i=0; i<num_states; i++){
        if((i>>qubit) & 1){
            prob_of_one += norm(state_vector[i]);
        }
    }

    static random_device rd;
    static mt19937 gen(rd());
    bernoulli_distribution dist(prob_of_one);
    int measurement = dist(gen);

    double norm_factor = measurement == 1 ? sqrt(prob_of_one) : sqrt(1.0-prob_of_one);

    for(int i=0; i<num_states; i++){
        bool bit_is_one = ((i>>qubit) & 1);

        if(bit_is_one == measurement){
            state_vector[i] /= norm_factor;
        }else{
            state_vector[i] = 0.0;
        }
    }

    cout << "Measurement qubit " << qubit << " and got: " << measurement << "\n";
    addCircuit(qubit,"M");
    return measurement;
}

//Helper functions
vector<string> QuantumCircuitBase::generateBasisStates(int n){
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

void QuantumCircuitBase::displayGraph() {
    // Step 1: Write data to a temporary file
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

    remove("prob_data.dat");
}

void QuantumCircuitBase::displayHeatMap() {
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

    remove("prob_data.dat");
}

void QuantumCircuitBase::printProbabilities(){
    vector<string> basis_states = generateBasisStates(qubit_count);

    cout << fixed << setprecision(6);
    cout << qubit_count << "-Qubit Measurement Results" << "\n";
    for(size_t i = 0; i<state_vector.size(); ++i) {
        double prob = norm(state_vector[i]);
        if(prob >= prob_threshold) cout << "Probability of |" << basis_states[i] << ">: " << prob << "\n";
    }
    cout << "----------------------------\n";
    // displayGraph();
}

void QuantumCircuitBase::printState() {
    cout << "Current State Vector" << "\n";
    vector<string>basis_states = generateBasisStates(qubit_count);
    for(size_t i=0; i<state_vector.size(); i++){
        cout << "|" << basis_states[i] << "> :" << state_vector[i] << "\n";
    }
}


