#include "Grid.h"

Grid::Grid(float xBound, float yBound, float zBound, float h, sim_state_t s){
	posVec = s.x;
	n = s.n;
	xDim = ceil(xBound / h);
	yDim = ceil(yBound / h);
	zDim = ceil(zBound / h);
	totalCells = xDim * yDim * zDim;
	grid = new vector<int>[totalCells];
	for (int i = 0; i < totalCells; i++){
		grid[i] = vector<int>();
	}
}

Grid::~Grid(){
	delete[] grid;
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
		float x = posVec[3 * i];
		float y = posVec[3 * i + 1];
		float z = posVec[3 * i + 2];

		int index = calcIndex(x, y, z);
		grid[index].push_back(i);
	}
}


/* Get neighbors for a particle */
vector<int> Grid::getNeighbors(int i) {
	float x = posVec[3 * i];
	float y = posVec[3 * i + 1];
	float z = posVec[3 * i + 2];

	int gridPos_x = floor(x / cutoff);
	int gridPos_y = floor(y / cutoff);
	int gridPos_z = floor(z / cutoff);

	vector<int> neighborInds = vector<int>();

	for (int a = gridPos_x - 1; a <= gridPos_x + 1; a++){
		for (int b = gridPos_y - 1; b <= gridPos_y + 1; b++){
			for (int c = gridPos_z - 1; c <= gridPos_z + 1; c++){
				if (isValidPos(a, b, c))
				{
					int grid_index = flatten(a, b, c);
					for (int d = 0; d < grid[grid_index].size(); d++)
					{
						int other_particle_index = grid[grid_index][d];
						float distance = getDistance(i, other_particle_index);
						if (distance < cutoff && other_particle_index > i){ // dont need duplicate neighbors
							neighborInds.push_back(other_particle_index);
						}
					}
				}
			}
		}
	}
}

/*  PRIVATE METHODS  */


float Grid::getDistance(int p1_index, int p2_index){
	float x_dist = posVec[3 * p1_index] - posVec[3 * p2_index];
	float y_dist = posVec[3 * p1_index + 1] - posVec[3 * p2_index + 1];
	float z_dist = posVec[3 * p1_index + 2] - posVec[3 * p2_index + 2];
	return sqrt(pow(x_dist, 2) + pow(y_dist, 2) + pow(z_dist, 2));
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