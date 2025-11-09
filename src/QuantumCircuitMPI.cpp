#include <mpi.h>
#include <stdexcept>
#include <cmath>
#include <MimiQ/QuantumCircuitMPI.h>
#include <MimiQ/QuantumGates.h>
using namespace std;

QuantumCircuitMPI::QuantumCircuitMPI(int n) : QuantumCircuitBase(n) {}

void QuantumCircuitMPI::applySingleQubitOp(int target_qubit, function<void(complex<double>&,complex<double>&)> op) {
    // if(target_qubit<0 || target_qubit>=qubit_count) throw out_of_range("Target qubit is out of range");
    
    int rank = 0; int size = 1;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    size_t N = state_vector.size();
    if (rank == 0) {
        if (target_qubit < 0 || target_qubit >= qubit_count)
            throw out_of_range("Target qubit is out of range");
        if ((1ULL << qubit_count) != N)
            throw runtime_error("state_vector size != 2^qubit_count");
    }
    

    // Send size of state vector and qubit count to all processes
    MPI_Bcast( &N , 1 , MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);
    MPI_Bcast(&qubit_count, 1, MPI_INT, 0, MPI_COMM_WORLD);


    const size_t block_size = 1ULL << target_qubit;
    const size_t stride =  block_size << 1;
    
    const size_t stride_count = N / stride;

    // store number of stridesper rank
    vector<int> block_per_rank(size), block_displs(size);
    {
        size_t base = stride_count / size;
        size_t rem = stride_count % size;

        for(int r=0;r<size;r++) {
            block_per_rank[r] = base + (r < (int)rem ? 1 : 0);
        }

        block_displs[0] = 0;
        for(int r=1;r<size;r++) {
            block_displs[r] = block_displs[r-1] + block_per_rank[r-1];
        }
    }

    vector<int> counts_elems(size), displs_elems(size);
    for(int r = 0;r<size;r++) {
        counts_elems[r] = block_per_rank[r] * (int)stride;
        displs_elems[r] = block_displs[r] * (int)stride;
    }


    int local_elems = counts_elems[rank]; // may be zero
    vector<complex<double>> local_buf; 
    if (local_elems > 0) local_buf.resize(local_elems);

    complex<double> *sendptr = nullptr;
    if(rank == 0) sendptr = state_vector.data();

    
    MPI_Scatterv( 
        sendptr , 
        counts_elems.data() , 
        displs_elems.data() , 
        MPI_CXX_DOUBLE_COMPLEX ,
        (local_elems > 0 ? local_buf.data() : nullptr) , 
        local_elems , 
        MPI_CXX_DOUBLE_COMPLEX ,
        0 , 
        MPI_COMM_WORLD
    );

    for(int offset = 0; offset < local_elems; offset += (int)stride){
        for(int j=0;j<block_size;j++){
            op(local_buf[offset + j], local_buf[offset + j + block_size]);
        }
    }

    MPI_Gatherv( 
    (local_elems > 0 ? local_buf.data() : nullptr) , 
    local_elems , 
    MPI_CXX_DOUBLE_COMPLEX , 
    sendptr ,
    counts_elems.data() , 
    displs_elems.data() , 
    MPI_CXX_DOUBLE_COMPLEX , 
    0 , MPI_COMM_WORLD);

}

void QuantumCircuitMPI::applyControlledQubitOp(int control_qubit, int target_qubit, function<void(complex<double>&, complex<double>&)> op) {
    int rank = 0; int size = 1;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    size_t N = state_vector.size();

    if (rank == 0) {
        if (target_qubit < 0 || target_qubit >= qubit_count)
            throw out_of_range("Target qubit is out of range");
        if ((1ULL << qubit_count) != N)
            throw runtime_error("state_vector size != 2^qubit_count");
    }
    

    // Send size of state vector and qubit count to all processes
    MPI_Bcast( &N , 1 , MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);
    MPI_Bcast(&qubit_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // size_t control_mask = 1ULL << control_qubit;
    const size_t block_size = 1ULL << target_qubit;
    const size_t control_mask = 1ULL << control_qubit;
    const size_t stride = block_size << 1;

    const size_t stride_count = N / stride;

    

     vector<int> block_per_rank(size), block_displs(size);
    {
        size_t base = stride_count / size;
        size_t rem = stride_count % size;

        for(int r=0;r<size;r++) {
            block_per_rank[r] = base + (r < (int)rem ? 1 : 0);
        }

        block_displs[0] = 0;
        for(int r=1;r<size;r++) {
            block_displs[r] = block_displs[r-1] + block_per_rank[r-1];
        }
    }

    vector<int> counts_elems(size), displs_elems(size);
    for(int r = 0;r<size;r++) {
        counts_elems[r] = block_per_rank[r] * (int)stride;
        displs_elems[r] = block_displs[r] * (int)stride;
    }


    int local_elems = counts_elems[rank]; // may be zero
    vector<complex<double>> local_buf; 
    if (local_elems > 0) local_buf.resize(local_elems);

    complex<double> *sendptr = nullptr;
    if(rank == 0) sendptr = state_vector.data();

    MPI_Scatterv(
        sendptr,
        counts_elems.data(),
        displs_elems.data(),
        MPI_CXX_DOUBLE_COMPLEX,
        (local_elems > 0 ? local_buf.data() : nullptr),
        local_elems,
        MPI_CXX_DOUBLE_COMPLEX,
        0,
        MPI_COMM_WORLD
    );

    for(int offset = 0; offset < local_elems; offset += (int)stride){
        for(int j=0;j<block_size;j++){
            if(((offset+j)&control_mask)!=0) op(local_buf[offset + j], local_buf[offset + j + block_size]);
        }
    }

    MPI_Gatherv(
        (local_elems > 0 ? local_buf.data() :nullptr),
        local_elems,
        MPI_CXX_DOUBLE_COMPLEX,
        sendptr,
        counts_elems.data(),
        displs_elems.data(),
        MPI_CXX_DOUBLE_COMPLEX,
        0,
        MPI_COMM_WORLD
    );
}