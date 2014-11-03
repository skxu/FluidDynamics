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
  glutPostRedisplay();
}

void keyUp (unsigned char key, int x, int y) {

}

void initShaderParams() {
  // Initialize shaders
  vertexshader = initshaders(GL_VERTEX_SHADER, "shaders/light.vert.glsl") ;
  fragmentshader = initshaders(GL_FRAGMENT_SHADER, "shaders/light.frag.glsl") ;
  shaderprogram = initprogram(vertexshader, fragmentshader) ; 
  enablelighting = glGetUniformLocation(shaderprogram,"enablelighting") ;
  lightpos = glGetUniformLocation(shaderprogram,"lightposn") ;       
  lightcol = glGetUniformLocation(shaderprogram,"lightcolor") ;       
  numused = glGetUniformLocation(shaderprogram,"numused") ;       
  ambientcol = glGetUniformLocation(shaderprogram,"ambient") ;       
  diffusecol = glGetUniformLocation(shaderprogram,"diffuse") ;       
  specularcol = glGetUniformLocation(shaderprogram,"specular") ;             
  shininesscol = glGetUniformLocation(shaderprogram,"shininess") ;
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void initValues() {
  // Scren values
  w = 800, h = 800;

  // Camera values
  eye = vec3(2.0, 0.0, 0.0);
  up = vec3(0.0, 1.0, 0.0);
  center = vec3(0.0, 0.0, 0.0);
  //
}

int main (int argc, char* argv[]) {
  initValues();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow("Fluid Simulation");
  initShaderParams();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  glutKeyboardUpFunc(keyUp);
  glutReshapeWindow(w,h);
  glutIdleFunc(idleFunc);
  glutMainLoop();
  return 0;
}
