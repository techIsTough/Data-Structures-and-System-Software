# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

# Target executable name
# Create with 'mingw32-make' on Windows
TARGET = DatabaseTester

# Source files
SOURCES = AVL_Database.cpp db_driver.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Header files
HEADERS = AVL_Database.hpp

# Default target
all: $(TARGET)

# Rule to link the target executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile source files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $<

# Clean up build files
# This revision works on Windows, run with 'mingw32-make clean'
clean:
	del /F $(OBJECTS) $(TARGET).exe

# Phony targets
.PHONY: all clean