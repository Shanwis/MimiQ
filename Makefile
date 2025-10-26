# Makefile for Quantum Simulator and Benchmark
CXX = g++
CXXFLAGS = -Iinclude -fopenmp

LIBDIR = lib
LIBNAME = libMimiQ.a
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
$(shell $(call MKDIR,$(LIBDIR)))

LIB_SRCS = $(wildcard $(SRCDIR)/*.cpp)
LIB_OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(LIB_SRCS))

# Target 1: The Interactive Simulator or other programs
PROGRAM ?= interactive_cli.cpp
TARGET_RUN = $(BINDIR)/$(PROGRAM:.cpp=)
MAIN_RUN_OBJ = $(OBJDIR)/$(PROGRAM:.cpp=.o)

# Target 2: The Benchmark Program
TARGET_BENCH = $(BINDIR)/Quantum_Benchmark
MAIN_BENCH_SRC = Benchmark.cpp
MAIN_BENCH_OBJ = $(OBJDIR)/$(MAIN_BENCH_SRC:.cpp=.o)

.PHONY: all clean run benchmark lib

all: lib $(TARGET_RUN) $(TARGET_BENCH)

$(TARGET_RUN): $(MAIN_RUN_OBJ) lib
	$(CXX) $(CXXFLAGS) -o $@ $(MAIN_RUN_OBJ) -L$(LIBDIR) -lMimiQ

$(TARGET_BENCH): $(MAIN_BENCH_OBJ) lib
	$(CXX) $(CXXFLAGS) -o $@ $(MAIN_RUN_OBJ) -L$(LIBDIR) -lMimiQ

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

lib: $(LIBDIR)/$(LIBNAME)

$(LIBDIR)/$(LIBNAME): $(LIB_OBJS)
	ar rcs $@ $^

run: $(TARGET_RUN)
	./$(TARGET_RUN)

benchmark: $(TARGET_BENCH)
	./$(TARGET_BENCH)

clean:
	-$(call RM,$(OBJDIR))
	-$(call RM,$(BINDIR))
	-$(call RM,$(LIBDIR))