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
#endif