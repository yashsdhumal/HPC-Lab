CXX = g++
CXXFLAGS = -O2 -std=c++17 -Wall -Wextra -fopenmp

all: prime

prime: prime.cpp
	$(CXX) $(CXXFLAGS) prime.cpp -o prime

run: prime
	./prime

clean:
	rm -f prime prime.exe
