CC = g++ -fopenmp
TARGET = solveFluid

default: solveFluid

solveFluid: *.cpp
	$(CC) -O2 -msse3 *.cpp -o solveFluid

clean:
	$(RM) *.o  *.h.gch solveFluid