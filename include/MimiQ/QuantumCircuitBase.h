#ifndef QUANTUMCIRCUITBASE_H
#define QUANTUMCIRCUITBASE_H

#include<vector>
#include<map>
#include<complex>
#include<string>
#include<functional>

class QuantumCircuitBase {
protected:
    //member var
    int qubit_count;
    std::vector<std::complex<double>> state_vector;

    //Circuit
    std::vector<std::string> circuit;

    //Add to the ASCII representation
    void addCircuit(int qubit,const std::string &gate);
    void addCircuit(int qubit1,const std::string &gate1, int qubit2, const std::string &gate2);
    //this aligns the columns of the circuit to look nice
    void alignCircuitColumns();

    //For performing operations
    //for single qubit operations
    virtual void applySingleQubitOp(int target_qubit, std::function<void(std::complex<double>&,std::complex<double>&)> op);
    //For two qubit operations
    virtual void applyTwoQubitOp(int qubit_1, int qubit_2, std::function<void(std::complex<double>&,std::complex<double>&, std::complex<double>&,std::complex<double>&)> op);
    //For controlled two qubit operations
    virtual void applyControlledQubitOp(int control_qubit, int target_qubit, std::function<void(std::complex<double>&, std::complex<double>&)> op);

public:
    //Constructor
    QuantumCircuitBase(int n);
    virtual ~QuantumCircuitBase() = default;

    //Public gate methods
    //Hadamard Gate
    virtual void H(int target_qubit);
    //Pauli gates
    virtual void X(int target_qubit);
    virtual void Z(int target_qubit);
    virtual void Y(int target_qubit);
    //Phase gates
    virtual void S(int target_qubit);
    virtual void Sdg(int target_qubit);
    virtual void T(int target_qubit);
    virtual void Tdg(int target_qubit);
    virtual void P(int target_qubit, const double theta);
    //Rotation gates
    virtual void Rz(int target_qubit, const double theta);
    virtual void Rx(int target_qubit, const double theta);
    virtual void Ry(int target_qubit, const double theta);
    //Controlled gates for above ones
    virtual void CX(int control_qubit, int target_qubit);
    virtual void CZ(int control_qubit, int target_qubit);
    virtual void CH(int control_qubit, int target_qubit);
    virtual void CY(int control_qubit, int target_qubit);
    virtual void CS(int control_qubit, int target_qubit);
    virtual void CSdg(int control_qubit, int target_qubit);
    virtual void CT(int control_qubit, int target_qubit);
    virtual void CTdg(int control_qubit, int target_qubit);
    virtual void CP(int control_qubit, int target_qubit, const double theta);
    virtual void CRx(int control_qubit, int target_qubit, const double theta);
    virtual void CRy(int control_qubit, int target_qubit, const double theta);
    virtual void CRz(int control_qubit, int target_qubit, const double theta);
    //Two qubit gates
    virtual void SWAP(int qubit_1, int qubit_2);
    virtual void iSWAP(int qubit_1, int qubit_2);

    //destructive measurement
    std::string collapse();
    //measurement for multiple runs
    std::map<std::string,int> run(int num_shots);
    //destructive measurement of a single qubit
    int measure_single_qubit(int qubit);
    //destructive measurement of a range of qubits
    std::string measure_range_of_qubits(const std::vector<int> &qubits);
    //measurement of a subset of qubits for multiple runs
    std::map<std::string,int> run_range_of_qubits(int num_shots, const std::vector<int> &qubits);
    void reset(int qubit);
    void resetAll(int qubit);

    // Helper to output probability amplitude
    std::complex<double> getProbAmplitude(const std::vector<std::complex<double>>& state_vector, int qubit){
        return state_vector[qubit];
    };

    double expectZ(vector<int> &q);
    //Helpers for outputing results
    void printState(); //prints the entire state
    void printCircuit(); //prints the entire circuit
    void printProbabilities(); //prints current probabilities above a threshold
    void displayGraph(); // displays a graph using GNUPlot for the probabilities above a threshold
    void displayHeatMap(); // heat map of the probabilities
};

#endif