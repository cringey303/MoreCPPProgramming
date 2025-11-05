# Compiler
CXX = g++

# Compiler flags
# -std=c++11: Use the C++11 standard
# -Wall: Show all warnings
# -g: Include debug symbols
CXXFLAGS = -std=c++11 -Wall -g

# The executable file we want to build
TARGET = calculator

# The default rule (what happens when you just type 'make')
all: $(TARGET)

# Rule to build the target
$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp

# Rule to clean up build files
clean:
	rm -f $(TARGET)