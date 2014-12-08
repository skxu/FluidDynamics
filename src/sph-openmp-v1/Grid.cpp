#include "Grid.h"

Grid::Grid(float xBound, float yBound, float zBound, float h, sim_state_t* s){
	posVec = s->x;
	n = s->n;
	xDim = ceil(xBound / h);
	yDim = ceil(yBound / h);
	zDim = ceil(zBound / h);
	totalCells = xDim * yDim * zDim;
	cutoff = h;
	cutoffSq = h*h;
	grid = vector<vector<int> >(totalCells, vector<int>());
	neighbors = vector<vector<int>*>();
	for (int i = 0; i < n; i++) neighbors.push_back(new vector<int>());
	speedOctopus = vector<vector<int> >(totalCells, vector<int>());
	for (int i = 0; i < totalCells; i++) fitOctopus(i);
}

Grid::~Grid(){
	for (int i = 0; i < n; i++) delete neighbors[i];
}

void Grid::cleanGrid(){
#pragma omp parallel for schedule(static, 32)
	for (int i = 0; i < totalCells; i++){
		grid[i].clear();
	}

#pragma omp parallel for schedule(static, 32)
	for (int i = 0; i < n; i++)
	{
		neighbors[i]->clear();
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
	setNeighbors();
}


/* Get neighbors for a particle */
vector<int>* Grid::getNeighbors(int i) {
	return neighbors[i];
}

/*  PRIVATE METHODS  */

/* For cell index i, calculate grid inds for 3x3x3 area around it */
void Grid::fitOctopus(int i) {
	int gridPos_z = i / (xDim * yDim);
	int gridPos_y = (i - gridPos_z * xDim * yDim) / xDim;
	int gridPos_x = i % xDim;

	for (int a = gridPos_x - 1; a <= gridPos_x + 1; a++){
		for (int b = gridPos_y - 1; b <= gridPos_y + 1; b++){
			for (int c = gridPos_z - 1; c <= gridPos_z + 1; c++){
				if (isValidPos(a, b, c))
				{
					speedOctopus[i].push_back(flatten(a, b, c));
				}
			}
		}
	}
}

/* Set neighbors for all particles */
void Grid::setNeighbors() {
  #pragma omp parallel for schedule(static, 8)
	for (int gridCell = 0; gridCell < grid.size(); gridCell++)
	{
		for (int a = 0; a < speedOctopus[gridCell].size(); a++)
		{
			int neighbor_grid_index = speedOctopus[gridCell][a];

			for (int b = 0; b < grid[gridCell].size(); b++)
			{
				int particleInd = grid[gridCell][b];
				float xi = posVec[4*particleInd+0];
				float yi = posVec[4*particleInd+1];
				float zi = posVec[4*particleInd+2];
				vector<int>* nVec = neighbors[particleInd];
				int c = 0;

				for (; c < grid[neighbor_grid_index].size(); c++)
				{
					int other_particle_index = grid[neighbor_grid_index][c];

					/* DISTANCE CALCULATION */

					float dx = xi - posVec[4*other_particle_index+0];
					float dy = yi - posVec[4*other_particle_index+1];
					float dz = zi - posVec[4*other_particle_index+2];					
					float d = dx*dx + dy*dy + dz*dz;

					/* END DISTANCE CALCULATION*/
					if (d < cutoffSq) {
						nVec->push_back(other_particle_index);
					} 
				}
			}
		}
	}
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