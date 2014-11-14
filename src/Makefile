CC = g++
TARGET = solveFluid

default: solveFluid

solveFluid: maintemp.cpp solver.cpp Particle.cpp vec3.cpp
	$(CC) -o solveFluid solver.cpp Particle.cpp vec3.cpp maintemp.cpp

clean:
	$(RM) *.o solveFluid