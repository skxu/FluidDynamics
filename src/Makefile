CC = g++
TARGET = solveFluid

default: solveFluid

solveFluid: main.cpp solver.cpp Particle.cpp vec3.cpp Grid.cpp
	$(CC) -o solveFluid solver.cpp Particle.cpp vec3.cpp Grid.cpp main.cpp

clean:
	$(RM) *.o  *.h.gch solveFluid