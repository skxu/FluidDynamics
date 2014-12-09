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

	grid = vector<vector<int> >(totalCells, vector<int>());
	gridCellsSize = 30;
	neighbors = new int[(neighborSize + 1) * n];

	speedOctopus = vector<vector<int>*>();
	for (int i = 0; i < totalCells; i++) speedOctopus.push_back(new vector<int>());
	for (int i = 0; i < totalCells; i++) fitOctopus(i);
}

void Grid::setup(std::map<std::string, cl_kernel> kernel_map, cl_vars_t cv)
{
	flatGrid = new int[(gridCellsSize + 1)*totalCells]();

	flatOctopus = new int[(27 + 1)*totalCells]();

	for (int i = 0; i < totalCells; i++)
	{
		int curInd = 0;
		int* flatP = flatOctopus + (27 + 1) * i;
		for (int j = 0; j < speedOctopus[i]->size() && curInd < 27; j++)
		{
			int nextElem = (*speedOctopus[i])[j];
			*(flatP + curInd) = nextElem;
			curInd++;
		}
		*(flatP + curInd) = -1;
	}

	cl_int err = CL_SUCCESS;
	g_flatGrid = clCreateBuffer(cv.context, CL_MEM_READ_WRITE,
		sizeof(int)*(gridCellsSize + 1)*totalCells, NULL, &err);
	CHK_ERR(err);

	g_neighbors = clCreateBuffer(cv.context, CL_MEM_READ_WRITE,
		sizeof(int)*(neighborSize + 1) * n, NULL, &err);
	CHK_ERR(err);

	g_posVec = clCreateBuffer(cv.context, CL_MEM_READ_WRITE,
		sizeof(float)*n * 4, NULL, &err);
	CHK_ERR(err);

	g_flatOctopus = clCreateBuffer(cv.context, CL_MEM_READ_WRITE,
		sizeof(int)*(27 + 1)*totalCells, NULL, &err);
	CHK_ERR(err);

	err = clEnqueueWriteBuffer(cv.commands, g_flatOctopus, true, 0, sizeof(int)*(27 + 1)*totalCells,
		flatOctopus, 0, NULL, NULL);
	CHK_ERR(err);
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
void Grid::setParticles(std::map<std::string, cl_kernel> kernel_map, cl_vars_t cv){
	cleanGrid();
	for (int i = 0; i < n; i++){
		float x = posVec[4 * i];
		float y = posVec[4 * i + 1];
		float z = posVec[4 * i + 2];
		int index = calcIndex(x, y, z);
		grid[index].push_back(i);
	}
	setNeighbors(kernel_map, cv);
}


/* Get neighbors for a particle */
int* Grid::getNeighbors(int i) {
	return neighbors + (neighborSize + 1) * i;
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
					speedOctopus[i]->push_back(flatten(a, b, c));
				}
			}
		}
	}
}

/* Set neighbors for all particles */
void Grid::setNeighbors(std::map<std::string, cl_kernel> kernel_map, cl_vars_t cv) {

	for (int i = 0; i < totalCells; i++)
	{
		int curInd = 0;
		int* flatP = flatGrid + (gridCellsSize + 1) * i;
		for (int j = 0; j < grid[i].size() && curInd < gridCellsSize; j++)
		{
			int nextElem = grid[i][j];
			*(flatP + curInd) = nextElem;
			curInd++;
		}
		*(flatP + curInd) = -1;
	}

	cl_int err = CL_SUCCESS;

	//copy data from host CPU to GPU
	err = clEnqueueWriteBuffer(cv.commands, g_flatGrid, true, 0, sizeof(int)*(gridCellsSize + 1)*totalCells,
		flatGrid, 0, NULL, NULL);
	CHK_ERR(err);

	err = clEnqueueWriteBuffer(cv.commands, g_posVec, true, 0, sizeof(float)*n * 4,
		posVec, 0, NULL, NULL);
	CHK_ERR(err);


	size_t global_work_size[1] = { n };
	size_t local_work_size[1] = { 128 };


	adjustWorkSize(global_work_size[0], local_work_size[0]);
	global_work_size[0] = std::max(local_work_size[0], global_work_size[0]);
	int left_over = 0;

	/* CS194: Implement radix sort here */

	/* get the kernels */

	cl_kernel neighborsK = kernel_map["neighbors"];


	/* These arguments to reassemble never change */

	err = clSetKernelArg(neighborsK, 0, sizeof(int), &xDim);
	CHK_ERR(err);
	err = clSetKernelArg(neighborsK, 1, sizeof(int), &yDim);
	CHK_ERR(err);
	err = clSetKernelArg(neighborsK, 2, sizeof(int), &zDim);
	CHK_ERR(err);
	err = clSetKernelArg(neighborsK, 3, sizeof(cl_mem), &g_neighbors);
	CHK_ERR(err);
	err = clSetKernelArg(neighborsK, 4, sizeof(int), &neighborSize);
	CHK_ERR(err);
	err = clSetKernelArg(neighborsK, 5, sizeof(cl_mem), &g_posVec);
	CHK_ERR(err);
	err = clSetKernelArg(neighborsK, 6, sizeof(cl_mem), &g_flatGrid);
	CHK_ERR(err);
	err = clSetKernelArg(neighborsK, 7, sizeof(int), &gridCellsSize);
	CHK_ERR(err);
	err = clSetKernelArg(neighborsK, 8, sizeof(float), &cutoff);
	CHK_ERR(err);
	err = clSetKernelArg(neighborsK, 9, sizeof(int), &n);
	CHK_ERR(err);
	err = clSetKernelArg(neighborsK, 10, sizeof(cl_mem), &g_flatOctopus);
	CHK_ERR(err);



	err = clEnqueueNDRangeKernel(cv.commands,
		neighborsK,
		1,//work_dim,
		NULL, //global_work_offset
		global_work_size, //global_work_size
		local_work_size, //local_work_size
		0, //num_events_in_wait_list
		NULL, //event_wait_list
		NULL //
		);
	CHK_ERR(err);
	err = clFinish(cv.commands);
	CHK_ERR(err);

	err = clEnqueueReadBuffer(cv.commands, g_neighbors, true, 0, sizeof(int)*(neighborSize + 1) * n,
		neighbors, 0, NULL, NULL);
	CHK_ERR(err);
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