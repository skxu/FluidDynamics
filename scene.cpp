#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glut.h>
#include "Transform.h"
#include "scene.h"

using namespace std ;
#include "variables.h"

void initializeScene() {
  addLight(1.0, 0.0, 0.0, 0.0, 0.9, 0.1, 0.1, 1.0);
  addLight(0.0, 1.0, 0.0, 0.0, 0.1, 0.9, 0.1, 1.0);
  addLight(0.0, 0.0, 1.0, 0.0, 0.1, 0.1, 0.9, 1.0);
}

void addLight(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
  if (numused < numLights) {
    lightposn[numused*4 + 0] = x;
    lightposn[numused*4 + 1] = y;
    lightposn[numused*4 + 2] = z;
    lightposn[numused*4 + 3] = w;
    lightcolor[numused*4 + 0] = r;
    lightcolor[numused*4 + 1] = g;
    lightcolor[numused*4 + 2] = b;
    lightcolor[numused*4 + 3] = a;
    numused++;
  }
}