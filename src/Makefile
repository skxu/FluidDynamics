CC = g++
TARGET = solveFluid

default: solveFluid

solveFluid: main.cpp solver.cpp Particle.cpp vec3.cpp
	$(CC) -o solveFluid solver.cpp Particle.cpp vec3.cpp main.cpp

clean:
	$(RM) *.o solveFluid