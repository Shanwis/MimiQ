SRC = src/QuantumCircuit.cpp src/main.cpp
CC = g++
CFLAG = -Iinclude
OBJ = $(SRC:.c=.o)
TARGET = Quantum_simulator

.PHONY: all clean run

all:$(TARGET)

$(TARGET):$(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run:$(TRAGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)