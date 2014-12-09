#include "Grid.h"

Grid::Grid(float xBound, float yBound, float zBound, float h, sim_state_t* s){
	posVec = s->x;
	n = s->n;
	xDim = ceil(4 * xBound / h);
	yDim = ceil(4 * yBound / h);
	zDim = ceil(4 * zBound / h);
	totalCells = xDim * yDim * zDim;
	cutoff = h;
	grid = vector<vector<int> >(totalCells, vector<int>());
	pNeighbors = new vector<int>*[n];
	cNeighbors = vector<vector<int>*>();
	for (int i = 0; i < totalCells; i++) cNeighbors.push_back(new vector<int>());
	speedOctopus = vector<vector<int>*>();
	for (int i = 0; i < totalCells; i++) speedOctopus.push_back(new vector<int>());
	for (int i = 0; i < totalCells; i++) fitOctopus(i);
}

Grid::~Grid(){
}

void Grid::cleanGrid(){
#pragma omp parallel for
	for (int i = 0; i < totalCells; i++){
		grid[i].clear();
	}

#pragma omp parallel for
	for (int i = 0; i < totalCells; i++)
	{
		cNeighbors[i]->clear();
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
		pNeighbors[i] = cNeighbors[index];
	}
	setNeighbors();
}


/* Get neighbors for a particle */
vector<int>* Grid::getNeighbors(int i) {
	return pNeighbors[i];
}

/*  PRIVATE METHODS  */

/* For cell index i, calculate grid inds for 3x3x3 area around it */
void Grid::fitOctopus(int i) {
	int gridPos_z = i / (xDim * yDim);
	int gridPos_y = (i - gridPos_z * xDim * yDim) / xDim;
	int gridPos_x = i % xDim;

	for (int a = 0; a <= 4; a++){
		for (int b = 0; b <= 4; b++){
			for (int c = 0; c <= 4; c++){
				if (isValidPos(gridPos_x-2+a, gridPos_y-2+b, gridPos_z-2+c, a, b, c))
				{
					speedOctopus[i]->push_back(flatten(gridPos_x-2+a, gridPos_y-2+b, gridPos_z-2+c));
				}
			}
		}
	}
}


/* Set neighbors for all particles */
void Grid::setNeighbors() {
#pragma omp parallel for schedule(static, 8)
	for (int gridCell = 0; gridCell < totalCells; gridCell++)
	{
		vector<int>* nVec = cNeighbors[gridCell];

		for (int a = 0; a < speedOctopus[gridCell]->size(); a++)
		{
			int neighbor_grid_index = (*speedOctopus[gridCell])[a];
			for (int c = 0; c < grid[neighbor_grid_index].size(); c++)
			{
				int other_particle_index = grid[neighbor_grid_index][c];
				nVec->push_back(other_particle_index);
			}
		}
	}
}



bool Grid::isValidPos(float gridPos_x, float gridPos_y, float gridPos_z, int a, int b, int c){
	return gridPos_x >= 0 && gridPos_x < xDim &&
		gridPos_y >= 0 && gridPos_y < yDim &&
		gridPos_z >= 0 && gridPos_z < zDim &&
		(((a != 0 && a != 4) || (b != 0 && b !=4)) &&
		((b != 0 && b != 4) || (c != 0 && c !=4)) &&
		((a != 0 && a != 4) || (c != 0 && c !=4)));
}

/* get cell index from particle coordinates */
int Grid::calcIndex(float x, float y, float z){
	int gridPos_x = floor(4*x / cutoff);
	int gridPos_y = floor(4*y / cutoff);
	int gridPos_z = floor(4*z / cutoff);
	return flatten(gridPos_x, gridPos_y, gridPos_z);
}

/* flatten indices */
int Grid::flatten(float gridPos_x, float gridPos_y, float gridPos_z)
{
	return gridPos_x + xDim * gridPos_y + (xDim * yDim) * gridPos_z;
}