#include <MaQrel/QuantumVisualization.h>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <iostream>

namespace QuantumVisualization{

    std::vector<std::string> generateBasisStates(int n){
        std::vector<std::string> basis_states;
        size_t num_states = 1<<n;
        for(size_t i = 0; i<num_states; i++){
            std::string basis = "";
            for(int j=n-1; j>=0; j--){
                basis += ((i>>j)&1)?'1':'0';
            }
            basis_states.push_back(basis);
        }
        return basis_states;
    }
    void printState(const std::vector<std::complex<double>>& state_vector, int qubit_count){
        std::cout << "Current State Vector" << "\n";
        std::vector<std::string>basis_states = generateBasisStates(qubit_count);
        for(size_t i=0; i<state_vector.size(); i++){
            std::cout << "|" << basis_states[i] << "> :" << state_vector[i] << "\n";
        }
    }

    void printCircuit(const std::vector<std::string> &circuit, int qubit_count){
        std::cout << "--- Circuit Diagram ---\n";
        for(int i=0; i<qubit_count; i++){
            std::cout << 'q' << i << " " << circuit[i] << "\n";
        }
        std::cout << "-----------------------\n";
    }

    void printProbabilities(const std::vector<std::complex<double>>& state_vector, int qubit_count){
        std::vector<std::string> basis_states = generateBasisStates(qubit_count);

        std::cout << std::fixed << std::setprecision(6);
        std::cout << qubit_count << "-Qubit Measurement Results" << "\n";
        for(size_t i = 0; i<state_vector.size(); ++i) {
            double prob = norm(state_vector[i]);
            if(prob >= PROB_THRESHOLD) std::cout << "Probability of |" << basis_states[i] << ">: " << prob << "\n";
        }
        std::cout << "----------------------------\n";
        // displayGraph();
    }

    void displayGraph(const std::vector<std::complex<double>>& state_vector, int qubit_count){
        // Step 1: Write data to a temporary file
        std::ofstream dataFile("prob_data.dat");
        if (!dataFile.is_open()) {
            std::cerr << "Error: Could not open data file for gnuplot." << std::endl;
            return;
        }

        std::vector<std::string> basis_states = generateBasisStates(qubit_count);
        for (size_t i = 0; i < state_vector.size(); ++i) {
            // Gnuplot format: "Label" Value
            double prob = norm(state_vector[i]);
            if(prob>=PROB_THRESHOLD){
                dataFile << "\"" << basis_states[i] << "\" " << prob << "\n";
            }
        }
        dataFile.close();

        #ifdef _WIN32
            std::string terminal = "set terminal qt size 800,600 font 'Verdana,10'; ";
        #else
            std::string terminal = "set terminal x11 size 800,600 font 'Verdana,10'; ";
        #endif


        // Step 2 & 3: Create a gnuplot command and execute it
        std::string gnuplot_command = 
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

        std::cout << "Displaying graph with gnuplot..." << std::endl;
        system(gnuplot_command.c_str());

        remove("prob_data.dat");
    }
    void displayHeatMap(const std::vector<std::complex<double>>& state_vector, int qubit_count){
        std::ofstream dataFile("prob_data.dat");

        int grid_size = 1 << (qubit_count/2);

        if (!dataFile.is_open()) {
            std::cerr << "Error: Could not open data file for gnuplot." << std::endl;
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
            std::string terminal = "set terminal qt size 800,600 font 'Verdana,10'; ";
        #else
            std::string terminal = "set terminal x11 size 800,600 font 'Verdana,10'; ";
        #endif


        // Step 2 & 3: Create a gnuplot command and execute it
        std::string gnuplot_command = 
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

        std::cout << "Displaying graph with gnuplot..." << std::endl;
        system(gnuplot_command.c_str());

        remove("prob_data.dat");
    }
}