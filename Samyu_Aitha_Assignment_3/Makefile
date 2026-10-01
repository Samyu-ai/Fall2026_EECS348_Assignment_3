CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

all: assignment3

assignment3: assignment3.cpp
	$(CXX) $(CXXFLAGS) assignment3.cpp -o assignment3

test: assignment3
	./assignment3 sample_test.txt > sample_actual.txt
	diff -u sample_expected.txt sample_actual.txt

clean:
	rm -f assignment3 sample_actual.txt

.PHONY: all test clean
