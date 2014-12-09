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
	int n)
{
  size_t idx = get_global_id(0);

  int particleInd = idx;

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

	for (int a = gridPos_x - 1; a <= gridPos_x + 1; a++){
		for (int b = gridPos_y - 1; b <= gridPos_y + 1; b++){
			for (int c = gridPos_z - 1; c <= gridPos_z + 1; c++){
				if (a >= 0 && a < xDim &&
					b >= 0 && b < yDim &&
					c >= 0 && c < zDim)
				{

				int neighbor_grid_index = a + xDim * b + (xDim * yDim) * c;

				__global int* neighborVec = grid + (gridCellSize+1)*neighbor_grid_index; 

				for (int d = 0; d < gridCellSize; d++)
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
			}
		}
	}

	nVec[curNeighborInd] = -1;
}