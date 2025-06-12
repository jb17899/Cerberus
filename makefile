# Compiler and flags
CXX = g++
CXXFLAGS = -INetworking  # Include Components folder for headers

# Set search paths for source files
VPATH = Networking

# Object files
OBJS = main.o Server.o 

# Build target
all: clean main

main: $(OBJS) 
	$(CXX) $^ -o $@

# Compile object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f *.o main
