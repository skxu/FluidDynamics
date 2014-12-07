#include "Grid.h"

Grid::Grid(float xBound, float yBound, float zBound, float h, sim_state_t* s){
	posVec = s->x;
	n = s->n;
	xDim = ceil(xBound / h);
	yDim = ceil(yBound / h);
	zDim = ceil(zBound / h);
	totalCells = xDim * yDim * zDim;
	cutoff = h;
	grid = vector<vector<int> >(totalCells, vector<int>());
	neighbors = vector<vector<int>*>();
	for (int i = 0; i < n; i++) neighbors.push_back(new vector<int>());
}

Grid::~Grid(){
	for (int i = 0; i < n; i++) delete neighbors[i];
}

void Grid::cleanGrid(){
	for (int i = 0; i < totalCells; i++){
		grid[i].clear();
	}
}


/* x = particle positions
   n = number particles */
void Grid::setParticles(){
	cleanGrid();
	for (int i = 0; i < n; i++){
		float x = posVec[3*i];
		float y = posVec[3*i+1];
		float z = posVec[3*i+2];
		int index = calcIndex(x, y, z);
		grid[index].push_back(i);
	}
	#pragma omp parallel for
	for (int i = 0; i < n; i++){
		setNeighbors(i);
	}
}


/* Get neighbors for a particle */
vector<int>* Grid::getNeighbors(int i) {
	return neighbors[i];
}

/*  PRIVATE METHODS  */


/* Set neighbors for a particle */
void Grid::setNeighbors(int i) {
	vector<int>* nVec = neighbors[i];
	nVec->clear();

	float x = posVec[3 * i];
	float y = posVec[3 * i + 1];
	float z = posVec[3 * i + 2];

	int gridPos_x = floor(x / cutoff);
	int gridPos_y = floor(y / cutoff);
	int gridPos_z = floor(z / cutoff);

	int k = 0;
	for (int a = gridPos_x - 1; a <= gridPos_x + 1; a++){
		for (int b = gridPos_y - 1; b <= gridPos_y + 1; b++){
			for (int c = gridPos_z - 1; c <= gridPos_z + 1; c++){
				k++;
				if (isValidPos(a, b, c))
				{
					int grid_index = flatten(a, b, c);
					for (int d = 0; d < grid[grid_index].size(); d++)
					{
						int other_particle_index = grid[grid_index][d];
						float distance2 = getDistance2(i, other_particle_index);
						if (distance2 < cutoff*cutoff) {
							nVec->push_back(other_particle_index);
						}
					}
				}
			}
		}
	}
}


float Grid::getDistance2(int p1_index, int p2_index){
	float x = posVec[3 * p1_index] - posVec[3 * p2_index];
	float y = posVec[3 * p1_index + 1] - posVec[3 * p2_index + 1];
	float z = posVec[3 * p1_index + 2] - posVec[3 * p2_index + 2];
	return x*x + y*y + z*z;
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