__kernel void neighbors(
	int xDim,
	int yDim,
	int zDim,
	int* neighbors,
	int neighborSize,
	float* posVec,
	int* flatGrid,
	int gridCellsSize,
	float h,
	int n)
{
  size_t idx = get_global_id(0);

  int particleInd = idx;

  int* nVec = neighbors + (neighborSize+1)*particleInd;
  int curNeighborInd = 0;

  float* pPos = posVec + 4 *particleInd;
  float x = *pPos;
  float y = *(pPos+1);
  float z = *(pPos+2);

  int gridCell = calcIndex(x, y, z);
  int gridPos_z = gridCell / (xDim * yDim);
  int gridPos_y = (gridCell - gridPos_z * xDim * yDim) / xDim;
  int gridPos_x = gridCell % xDim;

	for (int a = gridPos_x - 1; a <= gridPos_x + 1; a++){
		for (int b = gridPos_y - 1; b <= gridPos_y + 1; b++){
			for (int c = gridPos_z - 1; c <= gridPos_z + 1; c++){
				if (isValidPos(a, b, c))
				{

					int neighbor_grid_index = flatten(a,b,c);

					int* neighborVec = grid + (gridCellsSize+1)*neighbor_grid_index;

					for (int d = 0; neighborVec[d] != -1 && d < gridCellsSize; d++)
					{
						int other_particle_index = neighborVec[d];
						float* opPos = posVec + 4 * other_particle_index;
						float ox = *(opPos);
						float oy = *(opPos+1);
						float oz = *(opPos+2);

						float dx = ox - x;
						float dy - oy - y;
						float dz - oz - z;

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