# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

# Target executable name
TARGET = test_linked_calc

# Source files
SOURCES = tester.cpp linked_calc.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Header files
HEADERS = linked_calc.hpp

# Default target
all: $(TARGET)

# Rule to link the target executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile source files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $<

# Clean up build files
# This revision works on Windows
clean:
	del /F $(OBJECTS) $(TARGET).exe

# Phony targets
.PHONY: all clean