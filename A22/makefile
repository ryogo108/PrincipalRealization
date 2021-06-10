main: main.cpp realization.h CF.h
	c++ -std=c++11 -g -O2 -pthread -march=native main.cpp -o main -lntl -lgmp -lm

test_CF: test_CF.cpp CF.h
	c++ -std=c++11 -g -O2 -pthread -march=native test_CF.cpp -o test_CF -lntl -lgmp -lm
