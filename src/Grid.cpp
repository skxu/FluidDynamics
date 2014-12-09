#include "Grid.h"

Grid::Grid(float xBound, float yBound, float zBound, float h, sim_state_t* s){
	posVec = s->x;
	n = s->n;
	xDim = ceil(xBound / h);
	yDim = ceil(yBound / h);
	zDim = ceil(zBound / h);
	totalCells = xDim * yDim * zDim;
	cutoff = h;

	neighborSize = 10;

	grid = vector<vector<int>>(totalCells, vector<int>());
	gridPointers = new int*[totalCells];
	gridSizes = new int[totalCells];
	neighbors = new int[(neighborSize + 1) * n];
}

Grid::~Grid(){
}

void Grid::cleanGrid(){
#pragma omp parallel for
	for (int i = 0; i < totalCells; i++){
		grid[i].clear();
	}
}


/* x = particle positions
   n = number particles */
void Grid::setParticles(){
	cleanGrid();
	for (int i = 0; i < n; i++){
		float x = posVec[4 * i];
		float y = posVec[4 * i + 1];
		float z = posVec[4 * i + 2];
		int index = calcIndex(x, y, z);
		grid[index].push_back(i);
	}
	for (int i = 0; i < totalCells; i++)
	{
		gridPointers[i] = &(grid[i][0]);
		gridSizes[i] = grid[i].size();
	}
	setNeighbors();
}


/* Get neighbors for a particle */
int* Grid::getNeighbors(int i) {
	return neighbors + (neighborSize + 1) * i;
}

/*  PRIVATE METHODS  */



/* Set neighbors for all particles */
void Grid::setNeighbors() {

}



bool Grid::isValidPos(float gridPos_x, float gridPos_y, float gridPos_z){
	return gridPos_x >= 0 && gridPos_x < xDim &&
		gridPos_y >= 0 && gridPos_y < yDim &&
		gridPos_z >= 0 && gridPos_z < zDim;
}

/* get cell index from particle coordinates */
int Grid::calcIndex(float x, float y, float z){
	int gridPos_x = floor(x / cutoff);
	int gridPos_y = floor(y / cutoff);
	int gridPos_z = floor(z / cutoff);
	return flatten(gridPos_x, gridPos_y, gridPos_z);
}

/* flatten indices */
int Grid::flatten(float gridPos_x, float gridPos_y, float gridPos_z)
{
	return gridPos_x + xDim * gridPos_y + (xDim * yDim) * gridPos_z;
}