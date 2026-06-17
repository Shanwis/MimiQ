#ifndef QUANTUMGATES_H
#define QUANTUMGATES_H
#include <complex>
#include <cmath>

//This contains the gate functions used for operating on the state vectior matrix
namespace QuantumGates {

    inline const std::complex<double> I(0.0,1.0);

    inline auto Z_Function() {
        return [](std::complex<double> &a, std::complex<double> &b){ b*=-1.0; };
    }

    inline auto Y_Function() {
        return [](auto &a, auto &b){
            std::complex<double> b_old = b;
            b=I*a;
            a=-I*b_old;};
    }

    inline auto X_Function(){
        return [](auto &a, auto &b){swap(a,b);};
    }

    inline auto H_Function(){ 
        return [](auto &a, auto &b){
            std::complex<double> a_old = a;
            std::complex<double> b_old = b;
            a=(a_old+b_old)/std::sqrt(2);
            b=(a_old-b_old)/std::sqrt(2);
        };
    }

    inline auto Phase_Function(const std::complex<double> &phase){
        return [=](auto &a, auto &b){
            b*=phase;
        };
    };

    inline auto Rx_Function(const double theta){
        const std::complex<double> c = {std::cos(theta/2.0),0.0};
        const std::complex<double> s = {0.0,-std::sin(theta/2.0)};

        return [=](auto &a, auto &b){
            std::complex<double> a_old = a;
            std::complex<double> b_old = b;
            a = c*a_old + s*b_old;
            b = s*a_old + c*b_old;
        };
    }

    inline auto Ry_Function(const double theta){
        const std::complex<double> c = {std::cos(theta/2.0),0.0};
        const std::complex<double> s = {std::sin(theta/2.0),0.0};

        return [=](auto &a, auto &b){
            std::complex<double> a_old = a;
            std::complex<double> b_old = b;
            a = c*a_old - s*b_old;
            b = s*a_old + c*b_old;
        };
    }

    inline auto Rz_Function(const double theta){
        const std::complex<double> c = std::polar(1.0,-theta/2.0);
        const std::complex<double> s = std::polar(1.0,theta/2.0);

        return [=](auto &a, auto &b){
            a = a*c;
            b = b*s;
        };
    }

    inline auto SWAP_Function(){
        return [](auto &a, auto &b, auto &c, auto &d){
            std::swap(b,c);
        };
    }

    inline auto iSWAP_Function(){
        return [](auto &a, auto &b, auto &c, auto &d){
            std::swap(b,c);
            b = I*b;
            c = I*c;
        };
    }
}
// ---- Matrix-based gate representation (for fused/unfused matrix mode) ----

struct GateMatrix {
    std::complex<double> m[4]; // [a b; c d] row-major

    GateMatrix() {}
    GateMatrix(std::complex<double> a, std::complex<double> b,
               std::complex<double> c, std::complex<double> d)
        : m{a, b, c, d} {}
};

namespace GateMatrices {
    inline GateMatrix I()  { return {1, 0, 0, 1}; }
    inline GateMatrix H()  { return { M_SQRT1_2,  M_SQRT1_2,
                                      M_SQRT1_2, -M_SQRT1_2}; }
    inline GateMatrix X()  { return {0, 1, 1, 0}; }
    inline GateMatrix Y()  { return {0, std::complex<double>(0,-1), std::complex<double>(0,1), 0}; }
    inline GateMatrix Z()  { return {1, 0, 0, -1}; }
    inline GateMatrix S()  { return {1, 0, 0, std::complex<double>(0,1)}; }
    inline GateMatrix Sdg(){ return {1, 0, 0, std::complex<double>(0,-1)}; }
    inline GateMatrix T()  { return {1, 0, 0, std::polar(1.0, M_PI/4)}; }
    inline GateMatrix Tdg(){ return {1, 0, 0, std::polar(1.0, -M_PI/4)}; }

    inline GateMatrix P(double theta) {
        return {1, 0, 0, std::polar(1.0, theta)};
    }

    inline GateMatrix Rx(double theta) {
        auto c = std::cos(theta/2);
        auto s = std::sin(theta/2);
        std::complex<double> ni(0, -1);
        return {c, ni*s, ni*s, c};
    }

    inline GateMatrix Ry(double theta) {
        auto c = std::cos(theta/2);
        auto s = std::sin(theta/2);
        return {c, -s, s, c};
    }

    inline GateMatrix Rz(double theta) {
        auto p = std::polar(1.0, -theta/2);
        auto q = std::polar(1.0, theta/2);
        return {p, 0, 0, q};
    }
}

// b * a  (b applied after a in circuit order → reverse multiply)
inline GateMatrix multiply(const GateMatrix& b, const GateMatrix& a) {
    return {
        b.m[0]*a.m[0] + b.m[1]*a.m[2],
        b.m[0]*a.m[1] + b.m[1]*a.m[3],
        b.m[2]*a.m[0] + b.m[3]*a.m[2],
        b.m[2]*a.m[1] + b.m[3]*a.m[3]
    };
}

#endif