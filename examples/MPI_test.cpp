#include <iostream>
#include "include/QuantumCicuitMPI.h"
#include <mpi.h>

int main(int argc, char* argv[]){

    MPI_Init( &argc , &argv);
    int rank;
    MPI_Comm_rank( MPI_COMM_WORLD , &rank);
    QuantumCircuitMPI qc(4);
    qc.H(0);
    qc.X(2);
    qc.CX(2, 3);
    
    if(rank == 0){
        qc.printProbabilities();
        qc.printState();
    }
    
       

    MPI_Finalize();
    return 0;
}


// mpic++ MPI_test.cpp src/QuantumCircuitMPI.cpp src/QuantumCircuitBase.cpp src/QuantumVisualization.cpp -Iinclude -o test
// mpirun -np 4 ./test