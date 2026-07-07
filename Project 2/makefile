# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

# Target executable name
TARGET = FileSystemTester

# Source files
SOURCES = FileSystem.cpp FileSystemTester.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Header files
HEADERS = FileSystem.hpp

# Default target
all: $(TARGET)

# Rule to link the target executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile source files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $<

# Run with 'mingw32-make' on Windows
# Clean up build files
# This revision works on Windows, run with 'mingw32-make clean'
clean:
	del /F $(OBJECTS) $(TARGET).exe

# Phony targets
.PHONY: all clean