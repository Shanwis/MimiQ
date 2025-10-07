SRC = src/QuantumCircuit.cpp src/main.cpp
CC = g++
CFLAG = -fopenmp -Iinclude
OBJ = $(SRC:.cpp=.o)
TARGET = Quantum_simulator

.PHONY: all clean run

all:$(TARGET)

$(TARGET):$(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

run:$(TRAGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

