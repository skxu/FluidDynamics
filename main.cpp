#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glut.h>
#include "shaders.h"
#include <math.h>
#include <sys/time.h>

using namespace std;

// Main variables in the program.  
#define MAINPROGRAM 
#include "variables.h" 

void display(void) ;  // prototype for display function.

void reshape(int width, int height) {
  w = width;
  h = height;
  mat4 mv; // for lookat
  float aspect = w / (float) h, zNear = 0.001, zFar = 99.99;
  float fovy = 45.0;
  glMatrixMode(GL_PROJECTION);
  mv = glm::perspective(fovy,aspect,zNear,zFar);
  glLoadMatrixf(&mv[0][0]);
  glViewport(0,0,w,h);
}

void keyboard(unsigned char key, int x, int y) {
    
}

void idleFunc () {

}

void keyUp (unsigned char key, int x, int y) {

}

void init() {
  // Initialize shaders
  vertexshader = initshaders(GL_VERTEX_SHADER, "shaders/light.vert.glsl") ;
  fragmentshader = initshaders(GL_FRAGMENT_SHADER, "shaders/light.frag.glsl") ;
  shaderprogram = initprogram(vertexshader, fragmentshader) ; 
  enablelighting = glGetUniformLocation(shaderprogram,"enablelighting") ;
  lightpos = glGetUniformLocation(shaderprogram,"lightposn") ;       
  lightcol = glGetUniformLocation(shaderprogram,"lightcolor") ;       
  numusedcol = glGetUniformLocation(shaderprogram,"numused") ;       
  ambientcol = glGetUniformLocation(shaderprogram,"ambient") ;       
  diffusecol = glGetUniformLocation(shaderprogram,"diffuse") ;       
  specularcol = glGetUniformLocation(shaderprogram,"specular") ;       
  emissioncol = glGetUniformLocation(shaderprogram,"emission") ;       
  shininesscol = glGetUniformLocation(shaderprogram,"shininess") ;
  
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main (int argc, char* argv[]) {
  w = 800, h = 800;
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow("Fluid Simulation");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  glutKeyboardUpFunc(keyUp);
  glutReshapeWindow(w,h);
  glutIdleFunc(idleFunc);
  glutMainLoop();
  return 0;
}
