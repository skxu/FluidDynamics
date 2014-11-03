#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glut.h>
#include "shaders.h"
#include "Transform.h"
#include <math.h>
#include <sys/time.h>

using namespace std;

// Main variables in the program.  
#define MAINPROGRAM 
#include "variables.h" 

void display(void) ;  // prototype for display function.
void initializeScene(void);

void reshape(int width, int height) {
  w = width;
  h = height;
  mat4 mv; // for lookat
  float aspect = w / (float) h, zNear = 0.001, zFar = 99.99 ;
  glMatrixMode(GL_PROJECTION);
  mv = Transform::perspective(fovy,aspect,zNear,zFar);
  mv = glm::transpose(mv);
  glLoadMatrixf(&mv[0][0]);
  glViewport(0,0,w,h);
}


void keyboard(unsigned char key, int x, int y) {
  if (key == 'a') {
    Transform::left(3, eye, up);
  } else if (key == 'd') {
    Transform::left(-3, eye, up);
  }
}


void idleFunc ( ) {
  glutPostRedisplay();
}


void keyUp (unsigned char key, int x, int y) {

}

void mouse(int x, int y) {

}

void initShaderVars() {
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
  shininesscol = glGetUniformLocation(shaderprogram,"shininess") ;
}

void initVars() {
  w = 500;
  h = 500;
  eye = vec3(2,0,0);
  center = vec3(0,0,0);
  up = vec3(0,1,0);
  fovy = 45.0;
}

int main (int argc, char* argv[]) {
  initVars();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow("CS194: Fluid Simulation");
  initShaderVars();
  glEnable (GL_DEPTH_TEST);
  initializeScene();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  glutKeyboardUpFunc(keyUp);
  glutReshapeWindow(w,h);
  glutIdleFunc(idleFunc);
  glutPassiveMotionFunc(mouse);
  glutMainLoop();
  return 0;
}
