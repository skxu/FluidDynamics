#ifndef __INCLUDESCENE
#define __INCLUDESCENE

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>
#include "entity.h"

class Scene {
  public:
    Scene();
    void initializeScene();
    void update();
    void draw(glm::mat4 &mv);
  private:
    void addLight(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
    void renderLights();

    static const int numLights = 10;
    GLfloat lightposn [4*numLights] ; // Light Positions
    GLfloat lightcolor[4*numLights] ; // Light Colors
    GLfloat lightransf[4*numLights] ; // Lights transformed by modelview
    int numused ;                     // How many lights are used 

    std::vector<Entity> static_ents;
    std::vector<Entity> dynamic_ents;
    
};

#endif // __INCLUDESCENE