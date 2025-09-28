SRC = src/QuantumCircuit.cpp src/main.cpp
CC = g++
CFLAG = -Iinclude
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

# Compiler and source files
# CC = g++
# SRC = src/QuantumCircuit.cpp src/main.cpp

# # --- MODIFICATIONS START HERE ---

# # Use the python3-config tool to get the necessary flags
# PYTHON_CFLAGS = -I"C:/Program Files/Python312/Include" -I"C:\Users\elhan\AppData\Roaming\Python\Python312\site-packages\numpy\_core\include"
# PYTHON_LDFLAGS = -L"C:/Program Files/Python312/libs" -lpython312

# # Add the Python CFLAGS to your existing CFLAGS
# # NOTE: I've renamed your CFLAG to the standard CFLAGS (with an 'S')
# CFLAGS = -Iinclude $(PYTHON_CFLAGS)
# LDFLAGS = $(PYTHON_LDFLAGS)

# # --- MODIFICATIONS END HERE ---

# # Object files and target executable name
# OBJ = $(SRC:.cpp=.o)
# TARGET = Quantum_simulator

# .PHONY: all clean run

# all: $(TARGET)

# # Linking step: Added $(LDFLAGS) to link against the Python library
# $(TARGET): $(OBJ)
# 	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# # Compilation step: Uses the updated CFLAGS variable
# %.o: %.cpp
# 	$(CC) $(CFLAGS) -c $< -o $@

# # Run step: Fixed a typo (TRAGET -> TARGET)
# run: $(TARGET)
# 	./$(TARGET)

# clean:
# 	rm -f $(OBJ) $(TARGET)