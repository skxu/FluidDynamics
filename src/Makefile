CC = g++ -fopenmp
TARGET = solveFluid

default: solveFluid

solveFluid: *.cpp
	$(CC) -O2 -ftree-vectorize -ftree-vectorizer-verbose=10 -msse3 -msse2 -msse *.cpp -o solveFluid

clean:
	$(RM) *.o  *.h.gch solveFluid