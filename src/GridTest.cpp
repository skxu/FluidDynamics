#include "Grid.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char* argv[])
{
    Grid g = Grid(16,16,16,4);
    Particle* p1 = new Particle();
    p1->pos = vec3(5,1,1);
    g.setParticle(p1);
    //printf("Position1 is : (%f,%f,%f)",p1->gridPos.x,p1->gridPos.y,p1->gridPos.z);

    Particle* p2 = new Particle();
    p2->pos = vec3(0,0,0);
    g.setParticle(p2);
    //printf("Position2 is : (%f,%f,%f)\n",p2->gridPos.x,p2->gridPos.y,p2->gridPos.z);

    Particle* p3 = new Particle();
    p3->pos = vec3(4,1,1);
    g.setParticle(p3);

    g.setNeighbors(p1);
    g.setNeighbors(p2);
    g.setNeighbors(p3);
    //printf("%f,%f,%f\n",p1->neighbors[0][0]->pos.x,p1->neighbors[0][0]->pos.y,p1->neighbors[0][0]->pos.z);
    //printf("%f,%f,%f\n",p2->neighbors[0][0]->pos.x,p2->neighbors[0][0]->pos.y,p2->neighbors[0][0]->pos.z);
    printf("%lu\n",p3->neighbors[0].size());
    return 0;
}