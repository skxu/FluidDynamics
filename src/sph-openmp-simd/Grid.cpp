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
	float vals[4] __attribute__((aligned(0x10000)));
  #pragma omp parallel for schedule(static, 8) private(vals)
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


				__m128 pPos = _mm_load_ps(posVec + 4 * particleInd);
				for (; c + 4 <= grid[neighbor_grid_index].size(); c += 4)
				{

					__m128 oPos1 = _mm_load_ps(posVec + 4 * grid[neighbor_grid_index][c]);
					__m128 oPos2 = _mm_load_ps(posVec + 4 * grid[neighbor_grid_index][c + 1]);
					__m128 oPos3 = _mm_load_ps(posVec + 4 * grid[neighbor_grid_index][c + 2]);
					__m128 oPos4 = _mm_load_ps(posVec + 4 * grid[neighbor_grid_index][c + 3]);

					__m128 dif1 = _mm_sub_ps(pPos, oPos1);
					__m128 dif2 = _mm_sub_ps(pPos, oPos2);
					__m128 dif3 = _mm_sub_ps(pPos, oPos3);
					__m128 dif4 = _mm_sub_ps(pPos, oPos4);

					__m128 dist1 = _mm_mul_ps(dif1, dif1);
					__m128 dist2 = _mm_mul_ps(dif2, dif2);
					__m128 dist3 = _mm_mul_ps(dif3, dif3);
					__m128 dist4 = _mm_mul_ps(dif4, dif4);

					__m128 dist = _mm_hadd_ps(_mm_hadd_ps(dist1, dist2), _mm_hadd_ps(dist3, dist4));

					_mm_store_ps(vals, dist);

					for (int i = 0; i < 4; i++) {
						if (vals[i] < cutoffSq) {
							nVec->push_back(grid[neighbor_grid_index][c + i]);
						}
					}
				} 
				for (; c < grid[neighbor_grid_index].size(); c++) {
					int other_particle_index = grid[neighbor_grid_index][c];

					__m128 oPos = _mm_load_ps(posVec + 4 * other_particle_index);
					__m128 dif = _mm_sub_ps(pPos, oPos);
					__m128 dist = _mm_mul_ps(dif, dif);

					_mm_store_ps(vals, dist);
					float d = vals[0] + vals[1] + vals[2];

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
	int gridPos_x = int(x / cutoff);
	int gridPos_y = int(y / cutoff);
	int gridPos_z = int(z / cutoff);
	return flatten(gridPos_x, gridPos_y, gridPos_z);
}

/* flatten indices */
int Grid::flatten(float gridPos_x, float gridPos_y, float gridPos_z)
{
	return gridPos_x + xDim * gridPos_y + (xDim * yDim) * gridPos_z;
}