#ifndef __GRID_H__
#define __GRID_H__

#include <vector>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <pmmintrin.h>
#include "omp.h"

#include "Variables.h"

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cassert>
#include <cmath>
#include <unistd.h>
#include "clhelp.h"

using namespace std;

std::map<std::string, cl_kernel>
kernel_map;


class Grid{
public:
	Grid(float xBound, float yBound, float zBound, float h, sim_state_t* s);
	~Grid();

	/* Call this to refresh the particles to the correct cells */
	void setParticles();

	/* Get the neighbors of particle i */
	int* getNeighbors(int i);
	int neighborSize;

private:
	vector<vector<int>*> speedOctopus;
	
	vector<vector<int>> grid;
	int* neighbors;


	/* h */
	float cutoff;

	/* Vector of particle positions from sim_state_t*/
	float* posVec;

	/* Number of particles */
	int n;
	int xDim;
	int yDim;
	int zDim;
	int totalCells;
	int gridCellsSize;
	void setNeighbors();

	/* Precalculate all the neighbors to a grid cell */
	void fitOctopus(int i);

	
	/* Remove everything from cells */
	void cleanGrid();

	/* Check if this is within the grid */
	bool isValidPos(float gridPos_x, float gridPos_y, float gridPos_z);

	/* Locate index of cell for this position */
	int calcIndex(float x, float y, float z);

	/* Go from 3d grid indices to vector index */
	int flatten(float gridPos_x, float gridPos_y, float gridPos_z);
};

#endif