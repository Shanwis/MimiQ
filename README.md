# MimiQ: A Quantum Circuit Simulator

[![Language](https://img.shields.io/badge/Language-C%2B%2B-blue.svg)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT)

A lightweight, modern C++ library for simulating quantum circuits. This project provides a core QuantumCircuit class to build, manipulate, and measure N-qubit systems. It is capable of simulating both superposition and entanglement using a full state vector representation.

The repository also includes a command-line application that serves as a demonstration of the library's features.


## Features

* **Core C++ Library**: A self-contained set of classes of quantum simulation that can be easily integrated into your own C++ projects :) .
* **N-Qubit Simulation**: Simulate a quantum system with any number of qubits ($N$).
* **State Vector Model**: Uses a single state vector of $2^N$ complex amplitudes to accurately model entanglement and superposition.
* **Rich Gate Set (there is more to add honetly)**:
    * **Single-Qubit**: Hadamard (H), Pauli-X, Pauli-Y, Pauli-Z, Phase (S), T and T-dagger.
    * **Multi-Qubit**: Controlled-NOT (CNOT)
* **Circuit Visualization**: Renders an ASCII diagram of the circuit you've built.
* **Probability Graph**: Creates a GNUplot for showing the probablility graphs for the circuit created.
* **Three Measurement Modes**:
    1.  **Probabilistic**: Display the probabilities of all possible outcomes.
    2. **State vectors**: Display the state vector.
    2.  **Collapse**: Simulate a real measurement by collapsing the wave function to a single, definite state.

---

## Getting Started

### Requirements
* A modern C++ compiler (C++11 or later), such as g++, Clang, or MSVC.

* The make build automation tool.

* GNUplot installed and added to your PATH.

### Installation & Setup
**On Linux & macOS**

On most Linux distributions and macOS, g++ and make are either pre-installed or can be easily installed through your system's package manager.

**On Debian/Ubuntu**: 
```
sudo apt-get install build-essential
sudo apt install gnuplot
```

**On macOS**: Install the Command Line Tools by running xcode-select --install in your terminal.

**On Windows**:
Windows does not come with make or g++ by default. The recommended way to get them is by installing MSYS2, which provides a complete GNU development environment.

Download and install MSYS2 from the official website: https://www.msys2.org/

After installation, open the MSYS2 MINGW64 shell from the Start Menu.

Install the make and g++ toolchain by running the following command in the shell:
```
pacman -S --needed base-devel mingw-w64-x86_64-toolchain
```

(Optional but recommended) Add the MinGW-w64 bin folder to your Windows PATH environment variable to run make and g++ from any terminal (like PowerShell or Command Prompt). The path is typically C:\msys64\mingw64\bin.

Go to SourceForge abd install GNUPlot and add to the environment variables. 


### Build and Run
1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/Shanwis/MimiQ.git](https://github.com/Shanwis/MimiQ.git)
    cd MimiQ
    ```

2.  **Run the make file**
    ```bash
    make
    ```

     To use the library in your own project copy the include and lib files to you project
    ```bash
    MyQuantumApp/
    ├── include/
    │   └── MimiQ/
    │       ├── QuantumCircuitBase.h
    │       ├── QuantumCircuitParallel.h
    │       └── QuantumCircuitSerial.h
    ├── lib/
    │   └── libMimiQ.a
    ├── src/
    │   └── main.cpp
    └── Makefile
    ```

    when executing the src/main.cpp add flags

    ```bash
    -Iinclude -fopenmp -Llib -lMimiQ
    ```

    To run the interative CLI we made for seeing the library in action:
    ```bash
    make run
    ```

    To try running a code using the library inside the directory
    ```bash
    make run PROGRAM=<program name>
    ```

    To run the benchmark program to compare the serial and parallel implementation:
    ```bash
    make benchmark
    ```

3. **After use**
    ```bash
    make clean
    ```
## Usage

### 1. Interactive CLI 

Once the program is running, it will first ask for the number of qubits you want to simulate. After that, you will be presented with the main menu.

```
--- Menu ---
1. Apply Gate (includes measurement of single qubits)
2. Display Circuit
3. Measure Circuit(Collapses)
4. Print State Vector (Debug)
5. Print probabilities (Debug)
6. Display probability graph
7. Display heat map representation
8. Exit
Your choice:
```

Simply enter the number corresponding to the action you want to take. When applying gates, the program will prompt you for the gate type and the target/control qubits.

## Example: Creating a Bell State (Entanglement)

Here's how to create the famous entangled Bell state $\frac{1}{\sqrt{2}}(|00\rangle + |11\rangle)$:

1.  Start the simulator with **2 qubits**.
2.  Choose option **1. Apply Gate (includes measurement of single qubits)**, then apply an **H** gate to qubit **0**.
3.  Choose option **1. Apply Gate (includes measurement of single qubits)** again, then apply a **C** (CNOT) gate with control qubit **0** and target qubit **1**.
4.  Choose option **2. Display Circuit**. The output will be:
    ```
    --- Circuit Diagram ---
    q0 [H][C]
    q1 ---[*]
    -----------------------
    ```
5.  Choose option **5. Print probabilities (Debug)**. The output will show the classic signature of entanglement:
    ```
    Probabilities of states.
    2-Qubit Measurement Results
    Probability of |00>: 0.500000
    Probability of |11>: 0.500000
    ----------------------------
    ```
6. Choose option **6. Display probability graph** The output will show the graph with probability:

    ![Probability graph for bell state](./photos/graphusinggnuplot.png)

7. Choose option **7. Display heat map representation** The output will show a heat map representation of the probabilities associated with the states.

    ![Heat map representation for bell state](./photos/heatmaprepbellstateMimiQ.png)

8. Choose option **3. Measure Circuit(Collapses)** the system collapses to a single state.

    ```bash
    Your choice: 3

    Measuring the current state.
    00
    ```
    Here it colllapsed to 00 state. The circuit will look like:
    ```bash
    --- Circuit Diagram ---
    q0 [H][C][M]
    q1 ---[*][M]
    -----------------------
    ```
    The probability also changes:
    ```bash
    Probabilities of states.
    2-Qubit Measurement Results
    Probability of |00>: 1.000000
    ----------------------------
    ```


This result demonstrates that a measurement will yield either `|00>` or `|11>` with equal probability, but never `|01>` or `|10>`. The fates of the two qubits are linked!

### 2. Benchmark

When you get it running it will ask for number of qubits and threads after which it will run the serial and parallel versions of the simulator for 500 randomly applied gates.

```bash
--- Quantum Simulator Benchmark ---
Enter the number of qubits (e.g., 10): 15
Enter the number of threads for the parallel version: 5

Preparing to run a random circuit with 500 gates on a 15-qubit system.

--- Running Serial Benchmark ---
Serial execution time: 0.251818 seconds

--- Running Parallel Benchmark (5 threads) ---
Parallel execution time: 0.0829637 seconds

-----------------------------------------------------------
Speedup: 3.03527x
--By Ashwin S, 2023BCS0044 & Elhan B Thomas, 2023BCS0119--
```

### 3. Executing Code in the directory

Taking the example of bell state  again

```bash
//have this saved as bellstate.cpp in the directory
#include "include/QuantumCircuitParallel.h"

int main(){
    QuantumCircuitParallel qc(2);

    qc.H(0);
    qc.CNOT(0,1);

    qc.printCircuit();
    qc.measureProbabilities();
}
```
Run it in the directory and you will get output:
```bash
--- Circuit Diagram ---
q0 [H][C]
q1 ---[*]
-----------------------
2-Qubit Measurement Results
Probability of |00>: 0.500000
Probability of |11>: 0.500000
```

## Project Structure

```bash
MimiQ/
├── include/
│   ├── QuantumCircuitBase.h
│   ├── QuantumCircuitParallel.h
│   └── QuantumCircuitSerial.h
├── photos
│   ├── graphusinggnuplot.png
│   └── heatmaprepbellstateMimiQ.png
├── src
│   ├── QuantumCircuitBase.cpp
│   ├── QuantumCircuitParallel.cpp
│   └── QuantumCircuitSerial.cpp
├── Benchmark.cpp
├── interactive_cli.cpp
├── LICENSE
├── Makefile
└── README.md

4 directories, 13 files
```

## Future Scope

The library is limited by the system's RAM due to the use of openMP, this could be resolved by using the power of distributed computing.

A version using MPI is being made for this.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.
