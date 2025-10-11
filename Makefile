# Makefile for Quantum Simulator and Benchmark
CXX = g++
CXXFLAGS = -Iinclude -fopenmp

SRCDIR = src
OBJDIR = obj
BINDIR = bin

ifeq ($(OS), WINDOWS_NT)
	MKDIR = if not exist $(subst /,\,$1) mkdir $(subst /,\,$1)
	RM = rmdir /s /q $(subst /,\,$1)
	RMFILE = del /q $(subst /,\.$1)
else
	MKDIR = mkdir -p $1
	RM = rm -rf $1
	RMFILE = rm -f $1
endif

$(shell $(call MKDIR,$(OBJDIR)))
$(shell $(call MKDIR,$(BINDIR)))

LIB_SRCS = $(wildcard $(SRCDIR)/*.cpp)
LIB_OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(LIB_SRCS))

# Target 1: The Interactive Simulator
TARGET_SIM = $(BINDIR)/Quantum_simulator
MAIN_SIM_SRC = main.cpp
MAIN_SIM_OBJ = $(OBJDIR)/$(MAIN_SIM_SRC:.cpp=.o)

# Target 2: The Benchmark Program
TARGET_BENCH = $(BINDIR)/Quantum_Benchmark
MAIN_BENCH_SRC = Benchmark.cpp
MAIN_BENCH_OBJ = $(OBJDIR)/$(MAIN_BENCH_SRC:.cpp=.o)

.PHONY: all clean run benchmark

all: $(TARGET_SIM) $(TARGET_BENCH)

$(TARGET_SIM): $(MAIN_SIM_OBJ) $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TARGET_BENCH): $(MAIN_BENCH_OBJ) $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET_SIM)
	./$(TARGET_SIM)

benchmark: $(TARGET_BENCH)
	./$(TARGET_BENCH)

clean:
	-$(call RM,$(OBJDIR))
	-$(call RM,$(BINDIR))