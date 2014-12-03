#ifndef __GRID_H__
#define __GRID_H__

#include <vector>

#include "Variables.h"

using namespace std;

class Grid{
public:
	Grid(float xBound, float yBound, float zBound, float h, sim_state_t s);
	~Grid();

	/* Call this to refresh the particles to the correct cells */
	void setParticles();

	/* Get the neighbors of particle i that have index higher than this particle */
	vector<int> getNeighbors(int i);

	/* Get distance between particles at these indices */
	float getDistance(int p1_index, int p2_index);

	/* Check if this is within the grid */
	bool isValidPos(float gridPos_x, float gridPos_y, float gridPos_z);

	/* Locate index of cell for this position */
	int calcIndex(float x, float y, float z);
private:
	vector<int>* grid;

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

	/* Remove everything from cells */
	void cleanGrid();

	/* Go from 3d grid indices to vector index */
	int flatten(float gridPos_x, float gridPos_y, float gridPos_z);
};

#endif