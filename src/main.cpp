
#include "../include/QuantumCircuit.h" 
#include "../include/matplotlibcpp.h"

namespace plt = matplotlibcpp;
// int main() {
//     cout << "Creating a 2-qubit circuit." << endl;
//     QuantumCircuit qc(2);

//     cout << "Applying Hadamard gate to qubit 0." << endl;
//     qc.H(0);

//     cout << "Applying NOT (X) gate to qubit 1." << endl;
//     qc.X(1);

//     cout << "Measuring the circuit..." << endl;
//     qc.measure();

//     return 0;
// }
void updateCircuit(vector<vector<char>>& circuit, char gate, int bit) {
    for(int i=0;i<circuit.size();i++) { 
        if(i != bit) circuit[i].push_back('-');
        else circuit[i].push_back(gate);
    }
}

void displayPlot(){
        // 1. DATA GENERATION
    // Your C++ program generates the data for the qubit states.
    std::vector<std::string> states = {"|00>", "|01>", "|10>", "|11>"};
    std::vector<double> percentages = {85.5, 4.2, 5.3, 5.0};

    // 2. CREATE THE PLOT
    // Set the figure size for better readability
    plt::figure_size(800, 600);

    // Create the bar plot using the percentage values
    plt::bar(percentages);

    // 3. CUSTOMIZE THE PLOT
    plt::title("Qubit Measurement Probabilities");
    plt::xlabel("Qubit State");
    plt::ylabel("Percentage (%)");
    
    // Set the y-axis to range from 0 to 100
    plt::ylim(0, 100);

    // Set the x-axis ticks to correspond to our state labels.
    // This is a key step for categorical data.
    // We create a vector of positions [0, 1, 2, 3] for the ticks.
    std::vector<double> ticks(states.size());
    for(size_t i = 0; i < states.size(); ++i) {
        ticks[i] = i;
    }
    plt::xticks(ticks, states);

    // 4. SAVE OR SHOW THE PLOT
    // std::cout << "Saving plot to qubit_plot_mpl.png..." << std::endl;
    // plt::save("qubit_plot_mpl.png");

    // Alternatively, to display the plot in a GUI window:
    plt::show();
}

void applyGate(QuantumCircuit& qc, int n, vector<vector<char>>& circuit) {
    cout << endl << "Enter the aphabet of the Gate you wish to apply.\n";
        cout << "X GATE (X)\n";
        cout << "Z GATE (Z)\n";
        cout << "HADAMARD GATE (H)\n";
        cout << "Your gate: ";
        char gate;
        cin >> gate;
        switch(toupper(gate)) {
            case 'X': 
            case 'Z':
            case 'H': cout << endl; break;
            default: cout << "Invalid Gate!!\n"; return;
        }
        int bit;
        cout << "Enter the qubit number: ";
        cin  >> bit;
        cout << endl;
        if(bit > n) {
            cout << "Entered number exeeded number of qubits!!";
            return;
        }


    qc.CNOT(0, 1);

    qc.printState();

    while(choice == 'y') {
        cout << "1. Apply Gate operation\n";
        cout << "2. Measure states\n";
        cout << "3.Display Circuit\n";
        cout << "Your choice: ";
        int ch;
        cin >> ch;
        
        switch(ch) {
            case 1: applyGate(qc, n, circuit); break;
            case 2: qc.measure(); choice = 'n'; displayPlot(); break;
            case 3: displayCircuit(circuit);
        }
    }
}

    return 0;
}