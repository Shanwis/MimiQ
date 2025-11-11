#ifndef VQA_H
#define VQA_H

#include "QuantumCircuitBase.h"
#include "QuantumGates.h"
#include <string>
#include <vector>
#include <utility>   // for std::pair
#include <random>

using namespace std;
using namespace QuantumGates;

/**
 * QNN – A basic templated quantum neural network circuit abstraction.
 * Template parameter Tem: numeric type for parameters (e.g. double, float).
 */
template<class Tem>
class QNN{
private:
    // string -> gate name
    // vector<Tem> -> gate parameters (numeric or symbolic)
    vector<pair<string, vector<int>>> gates;
    vector<Tem> params;
    int qubit_count;
    QuantumCircuitBase qc;
public:
    explicit QNN(int noqubits);
    ~QNN() = default;

    virtual Tem forward(Tem x,Tem y);
    virtual Tem forward(Tem x, Tem y, const vector<int> &qubits);
    virtual double backward(Tem x,Tem y,double lr=.01);
    // Single-qubit gates
    virtual void H(int target_qubit);
    virtual void X(int target_qubit);
    virtual void Z(int target_qubit);
    virtual void Y(int target_qubit);

    // Phase gates
    virtual void S(int target_qubit);
    virtual void Sdg(int target_qubit);
    virtual void T(int target_qubit);
    virtual void Tdg(int target_qubit);
    virtual void P(int target_qubit);

    // Rotation gates
    virtual void Rz(int target_qubit);
    virtual void Rx(int target_qubit);
    virtual void Ry(int target_qubit);

    // Controlled gates
    virtual void CX(int control_qubit, int target_qubit);
    virtual void CZ(int control_qubit, int target_qubit);
    virtual void CH(int control_qubit, int target_qubit);
    virtual void CY(int control_qubit, int target_qubit);
    virtual void CS(int control_qubit, int target_qubit);
    virtual void CSdg(int control_qubit, int target_qubit);
    virtual void CT(int control_qubit, int target_qubit);
    virtual void CTdg(int control_qubit, int target_qubit);
    virtual void CP(int control_qubit, int target_qubit);
    virtual void CRx(int control_qubit, int target_qubit);
    virtual void CRy(int control_qubit, int target_qubit);
    virtual void CRz(int control_qubit, int target_qubit);

    // Parameter initialization helper
    Tem init_param();

    // Debug helper
    void printCircuit() const;

};

#endif
