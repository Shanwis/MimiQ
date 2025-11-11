#include "../include/VQA.h"
#include <iostream>
#include <iomanip>

using namespace std;

// ===== Constructor =====
template<class Tem>
QNN<Tem>::QNN(int noqubits)
    :
      qubit_count(noqubits),
      qc(noqubits)
{
    gates.clear();
    params.clear();
    cout << "QNN initialized with " << noqubits << " qubits.\n";
}

template<class Tem>
double QNN<Tem>::backward(Tem x,Tem y,double lr){
    double y_out = forward(x,y);
    double err = y=y_out;
    for(int i=0;i<params.size();i++){
        QuantumCircuitBase qcp=qc,qcn=qc;
        params[i]+=M_PI/2;
        double ep = forward(x,y);
        params[i]-=M_PI;
        double en = forward(x,y);
        double d_theta = .5*(ep-en);
        params[i] = fmod(lr*err*d_theta,2*M_PI);
    }
    return err;
}

template<class Tem>
Tem QNN<Tem>::forward(Tem x, Tem y){
    for(pair<string,vector<int>> &p : gates){
        if (p.first == "H") {
            qc.H(p.second[0]);
        }
        else if (p.first == "X") {
            qc.X(p.second[0]);
        }
        else if (p.first == "Z") {
            qc.Z(p.second[0]);
        }
        else if (p.first == "Y") {
            qc.Y(p.second[0]);
        }
        else if (p.first == "S") {
            qc.S(p.second[0]);
        }
        else if (p.first == "Sdg") {
            qc.Sdg(p.second[0]);
        }
        else if (p.first == "T") {
            qc.T(p.second[0]);
        }
        else if (p.first == "P") {
            qc.P(p.second[0], params[p.second[1]]);
        }
        else if (p.first == "Rz") {
            qc.Rz(p.second[0], params[p.second[1]]);
        }
        else if (p.first == "Rx") {
            qc.Rx(p.second[0], params[p.second[1]]);
        }
        else if (p.first == "Ry") {
            qc.Ry(p.second[0], params[p.second[1]]);
        }
        else if (p.first == "CX") {
            qc.CX(p.second[0], p.second[1]);
        }
        else if (p.first == "CZ") {
            qc.CZ(p.second[0], p.second[1]);
        }
        else if (p.first == "CY") {
            qc.CY(p.second[0], p.second[1]);
        }
        else if (p.first == "CH") {
            qc.CH(p.second[0], p.second[1]);
        }
        else if (p.first == "CS") {
            qc.CS(p.second[0], p.second[1]);
        }
        else if (p.first == "CSdg") {
            qc.CSdg(p.second[0], p.second[1]);
        }
        else if (p.first == "CT") {
            qc.CT(p.second[0], p.second[1]);
        }
        else if (p.first == "CTdg") {
            qc.CTdg(p.second[0], p.second[1]);
        }
        else if (p.first == "CP") {
            qc.CP(p.second[0], p.second[1], params[p.second[2]]);
        }
        else if (p.first == "CRx") {
            qc.CRx(p.second[0], p.second[1], params[p.second[2]]);
        }
        else if (p.first == "CRy") {
            qc.CRy(p.second[0], p.second[1], params[p.second[2]]);
        }
        else if (p.first == "CRz") {
            qc.CRz(p.second[0], p.second[1], params[p.second[2]]);
        }
        else {
            cerr << "Unknown gate: " << p.first << endl;
        }
    }
    return qc.expectZ();
}

template<class Tem>
Tem QNN<Tem>::forward(Tem x, Tem y, const vector<int> &qubits){
    for(pair<string,vector<int>> &p : gates){
        if (p.first == "H") {
            qc.H(p.second[0]);
        }
        else if (p.first == "X") {
            qc.X(p.second[0]);
        }
        else if (p.first == "Z") {
            qc.Z(p.second[0]);
        }
        else if (p.first == "Y") {
            qc.Y(p.second[0]);
        }
        else if (p.first == "S") {
            qc.S(p.second[0]);
        }
        else if (p.first == "Sdg") {
            qc.Sdg(p.second[0]);
        }
        else if (p.first == "T") {
            qc.T(p.second[0]);
        }
        else if (p.first == "P") {
            qc.P(p.second[0], params[p.second[1]]);
        }
        else if (p.first == "Rz") {
            qc.Rz(p.second[0], params[p.second[1]]);
        }
        else if (p.first == "Rx") {
            qc.Rx(p.second[0], params[p.second[1]]);
        }
        else if (p.first == "Ry") {
            qc.Ry(p.second[0], params[p.second[1]]);
        }
        else if (p.first == "CX") {
            qc.CX(p.second[0], p.second[1]);
        }
        else if (p.first == "CZ") {
            qc.CZ(p.second[0], p.second[1]);
        }
        else if (p.first == "CY") {
            qc.CY(p.second[0], p.second[1]);
        }
        else if (p.first == "CH") {
            qc.CH(p.second[0], p.second[1]);
        }
        else if (p.first == "CS") {
            qc.CS(p.second[0], p.second[1]);
        }
        else if (p.first == "CSdg") {
            qc.CSdg(p.second[0], p.second[1]);
        }
        else if (p.first == "CT") {
            qc.CT(p.second[0], p.second[1]);
        }
        else if (p.first == "CTdg") {
            qc.CTdg(p.second[0], p.second[1]);
        }
        else if (p.first == "CP") {
            qc.CP(p.second[0], p.second[1], params[p.second[2]]);
        }
        else if (p.first == "CRx") {
            qc.CRx(p.second[0], p.second[1], params[p.second[2]]);
        }
        else if (p.first == "CRy") {
            qc.CRy(p.second[0], p.second[1], params[p.second[2]]);
        }
        else if (p.first == "CRz") {
            qc.CRz(p.second[0], p.second[1], params[p.second[2]]);
        }
        else {
            cerr << "Unknown gate: " << p.first << endl;
        }
    }
    return qc.expectZ();
}

// ===== Parameter initialization =====
template<class Tem>
Tem QNN<Tem>::init_param() {
    return Tem{1.2};   // safe zero-like default
}

// ===== Single-qubit gates =====
template<class Tem> void QNN<Tem>::H(int target)  { gates.push_back({"H", {target}}); }
template<class Tem> void QNN<Tem>::X(int target)  { gates.push_back({"X", {target}}); }
template<class Tem> void QNN<Tem>::Y(int target)  { gates.push_back({"Y", {target}}); }
template<class Tem> void QNN<Tem>::Z(int target)  { gates.push_back({"Z", {target}}); }

// ===== Phase gates =====
template<class Tem> void QNN<Tem>::S(int target)   { gates.push_back({"S",   {target}}); }
template<class Tem> void QNN<Tem>::Sdg(int target) { gates.push_back({"Sdg", {target}}); }
template<class Tem> void QNN<Tem>::T(int target)   { gates.push_back({"T",   {target}}); }
template<class Tem> void QNN<Tem>::Tdg(int target) { gates.push_back({"Tdg", {target}}); }

// ===== Parametric single-qubit gates =====
template<class Tem> void QNN<Tem>::P(int target) {
    Tem p = init_param();
    params.push_back(p);
    gates.push_back({"P", {target, (int)(params.size()-1)}});
}

template<class Tem> void QNN<Tem>::Rx(int target) {
    Tem p = init_param();
    params.push_back(p);
    gates.push_back({"Rx", {target, (int)(params.size()-1)}});
}

template<class Tem> void QNN<Tem>::Ry(int target) {
    Tem p = init_param();
    params.push_back(p);
    gates.push_back({"Ry", {target, (int)(params.size()-1)}});
}

template<class Tem> void QNN<Tem>::Rz(int target) {
    Tem p = init_param();
    params.push_back(p);
    gates.push_back({"Rz", {target, (int)(params.size()-1)}});
}

// ===== Controlled gates =====
template<class Tem> void QNN<Tem>::CX(int c, int t)   { gates.push_back({"CX", {c, t}}); }
template<class Tem> void QNN<Tem>::CZ(int c, int t)   { gates.push_back({"CZ", {c, t}}); }
template<class Tem> void QNN<Tem>::CH(int c, int t)   { gates.push_back({"CH", {c, t}}); }
template<class Tem> void QNN<Tem>::CY(int c, int t)   { gates.push_back({"CY", {c, t}}); }
template<class Tem> void QNN<Tem>::CS(int c, int t)   { gates.push_back({"CS", {c, t}}); }
template<class Tem> void QNN<Tem>::CSdg(int c, int t) { gates.push_back({"CSdg",{c, t}}); }
template<class Tem> void QNN<Tem>::CT(int c, int t)   { gates.push_back({"CT", {c, t}}); }
template<class Tem> void QNN<Tem>::CTdg(int c, int t) { gates.push_back({"CTdg",{c, t}}); }

// ===== Parametric controlled gates =====
template<class Tem> void QNN<Tem>::CP(int c, int t) {
    Tem p = init_param();
    params.push_back(p);
    gates.push_back({"CP", {c, t, (int)(params.size()-1)}});
}

template<class Tem> void QNN<Tem>::CRx(int c, int t) {
    Tem p = init_param();
    params.push_back(p);cout<<params.size()-1<<endl;
    gates.push_back({"CRx", {c, t, (int)(params.size()-1)}});
}

template<class Tem> void QNN<Tem>::CRy(int c, int t) {
    Tem p = init_param();
    params.push_back(p);
    gates.push_back({"CRy", {c, t, (int)(params.size()-1)}});
}

template<class Tem> void QNN<Tem>::CRz(int c, int t) {
    Tem p = init_param();
    params.push_back(p);
    gates.push_back({"CRz", {c, t, (int)(params.size()-1)}});
}

// ===== Debug print helper =====
template<class Tem>
void QNN<Tem>::printCircuit() const {
    cout << "\n--- Quantum Circuit ---\n";
    for (const auto& g : gates) {
        cout << setw(6) << g.first << " : ";
        for (auto v : g.second)
            cout << v << " ";
        cout << "\n";
    }
    cout << "-----------------------\n";
}

// ===== Explicit instantiation =====
template class QNN<double>;
template class QNN<float>;
template class QNN<long double>;
