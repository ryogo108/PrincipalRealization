main: src/main.cpp src/realization.h src/CF.h src/realization.cpp
	c++ -std=c++11 -g -O2 -pthread -march=native src/main.cpp src/realization.cpp -o main -lntl -lgmp -lm
R_test: test/R_test.cpp src/CF.h src/realization.h src/realization.cpp
	c++ -std=c++11 -g -O2 -pthread -march=native test/R_test.cpp src/realization.cpp -o R_test -lntl -lgmp -lm -lgtest -lgtest_main
