#include "Grid.h"

Grid::Grid(int x, int y, int z, int h){
    cutoff = h;
    xDim = x/h;
    yDim = y/h;
    zDim = z/h;
    for(int i = 0; i < (x*y*z)/(h*h*h); i++){
        grid.push_back(std::vector<Particle*>());
    }
}

void Grid::cleanGrid(){
    for(int i = 0; i < xDim*yDim*zDim; i++){
        grid[i].clear();
    }
}

pVec Grid::getParticles(int index){
    return grid[index];
}

void Grid::setParticle(Particle* p){
    p->neighbors = new std::vector<Particle*>();
    p->neighborDirections = new std::vector<vec3*>();
    vec3 gridPosition = calcGridPos(p);
    //printf("our grid position is: (%f,%f,%f)\n",gridPosition.x,gridPosition.y,gridPosition.z);
    p->gridPos = gridPosition;
    int index = calcIndex(gridPosition);
    //printf("with index %d\n\n",index);
    grid[index].push_back(p);
}

void Grid::setParticles(pVec particles){
    for(int i = 0; i < particles.size(); i++){
        setParticle(particles[i]);
    }
}

/*
void Grid::updateParticle(Particle* p){
    int index = calcIndex(p->gridPos);
    vec3 updatedGridPos = calcGridPos(p);
    if (index != calcIndex(updatedGridPos)){
        grid[index].pop_back(); //might/probably has a bug--probably need to clean entire grid when we update
        grid[calcIndex(updatedGridPos)].push_back(p);
    }
    p->gridPos = updatedGridPos;
}
*/

void Grid::setNeighbors(Particle *p){
    vec3 v;
    pVec particles;
    float distance;
    for (int i = p->gridPos.x-1; i < p->gridPos.x+2; i++){
        for (int j = p->gridPos.y-1; j < p->gridPos.y+2; j++){
            for (int k = p->gridPos.z-1; k < p->gridPos.z+2; k++){
                //printf("Checking coordinate %d,%d,%d\n",i,j,k);
                v = vec3(i,j,k);
                if (isValidPos(v)){
                    particles = getParticles(calcIndex(v));
                    for(int a = 0; a < particles.size(); a++){
                        distance = getDistance(p, particles[a]);
                        //printf("The distance is %f\n",distance);
                        if (distance <= (float) cutoff){
                            p->neighbors->push_back(particles[a]); //need to find out how to update neighbors, most likely clear neighbors list in particle
                            vec3 dVec = new vec3(); 
                            *dVec = p->pos-particles[a]->pos;
                            p->neighborDirections->push_back(dVec);
                        }
                    }
                }
            }
        }
    }
}

void Grid::setNeighbors(pVec pVector){
    for(int i = 0; i < pVector.size(); i++){
        setNeighbors(pVector[i]);
    }
}

pVec* Grid::getNeighbors(Particle *p){
    return p->neighbors;
}


float Grid::getDistance(Particle* p1, Particle* p2){
    return sqrt(pow(p2->pos.x - p1->pos.x, 2)+pow(p2->pos.y - p1->pos.y,2)+pow(p2->pos.z - p1->pos.z,2));
}

bool Grid::isValidPos(vec3 v){
    //printf("invalid being called\n");
    return v.x >= 0 && v.x < xDim && v.y >= 0 && v.y < yDim && v.z >= 0 && v.z < zDim;
}

vec3 Grid::calcGridPos(Particle* p){
    int i = 1;
    while (p->pos.x > i*cutoff){
        i++;
    }
    int j = 1;
    while (p->pos.y > j*cutoff){
        j++;
    }
    int k = 1;
    while (p->pos.z > k*cutoff){
        k++;
    }
    i--; j--; k--;
    return vec3(i,j,k);
}


int Grid::calcIndex(vec3 v){
    return v.x + (xDim * v.y) + (xDim * yDim * v.z);
}

int Grid::getCutoff(){
    return cutoff;
}