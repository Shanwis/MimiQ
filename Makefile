# Makefile for Quantum Simulator and Benchmark
CXX = g++
CXXFLAGS = -Iinclude -fopenmp

LIB_SRCS = $(wildcard src/*.cpp)
LIB_OBJS = $(LIB_SRCS:.cpp=.o)

# Target 1: The Interactive Simulator
TARGET_SIM = Quantum_simulator
MAIN_SIM_SRC = main.cpp
MAIN_SIM_OBJ = $(MAIN_SIM_SRC:.cpp=.o)

# Target 2: The Benchmark Program
TARGET_BENCH = Quantum_Benchmark
MAIN_BENCH_SRC = Testing.cpp
MAIN_BENCH_OBJ = $(MAIN_BENCH_SRC:.cpp=.o)

.PHONY: all clean run benchmark

all: $(TARGET_SIM) $(TARGET_BENCH)

$(TARGET_SIM): $(MAIN_SIM_OBJ) $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET_SIM) $^

$(TARGET_BENCH): $(MAIN_BENCH_OBJ) $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET_BENCH) $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET_SIM)
	./$(TARGET_SIM)

benchmark: $(TARGET_BENCH)
	./$(TARGET_BENCH)

clean:
	rm -f $(LIB_OBJS) $(MAIN_SIM_OBJ) $(MAIN_BENCH_OBJ) $(TARGET_SIM) $(TARGET_BENCH)