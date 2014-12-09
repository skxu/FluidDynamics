__kernel void neighbors(
	int xDim,
	int yDim,
	int zDim,
	__global int* neighbors,
	int neighborSize,
	__global float* posVec,
	__global int* grid,
	int gridCellSize,
	float h,
	int n,
	__global int* octopus)
{
  size_t idx = get_global_id(0);

  int particleInd = idx;

if (particleInd < n)
{

  __global int* nVec = neighbors + (neighborSize+1)*particleInd;
  int curNeighborInd = 0;

  __global float* pPos = posVec + 4 *particleInd;
  float x = *pPos;
  float y = *(pPos+1);
  float z = *(pPos+2);


	int gridPos_x = floor(x / h);
	int gridPos_y = floor(y / h);
	int gridPos_z = floor(z / h);

int gridCell = gridPos_x + xDim * gridPos_y + (xDim * yDim) * gridPos_z;

	__global int* octoleg = octopus + (27+1) * gridCell;
 
       for (int a = 0; a < 27 && octoleg[a] != -1; a++)
       {

				int neighbor_grid_index = octoleg[a];

				__global int* neighborVec = grid + (gridCellSize+1)*neighbor_grid_index; 

				for (int d = 0; d < gridCellSize && neighborVec[d] != -1; d++)
				{
					int other_particle_index = neighborVec[d];
					__global float* opPos = posVec + 4 * other_particle_index;
					float ox = *(opPos);
					float oy = *(opPos+1);
					float oz = *(opPos+2);

					float dx = ox - x;
					float dy = oy - y;
					float dz = oz - z;

					float dist = dx * dx + dy * dy + dz * dz;
					if (dist < h*h && curNeighborInd < neighborSize)
					{
						nVec[curNeighborInd] = other_particle_index;
						curNeighborInd++;
					}
                }

	}

	nVec[curNeighborInd] = -1;
}
}