#include <GL/glut.h>
#include <glm/glm.hpp>
#include "shaders.h"
#include "Transform.h"
#include "scene.h"

using namespace std;

// Main variables in the program.  
#define MAINPROGRAM 
#include "variables.h"

Scene* scene;

void display() {
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glm::mat4 mv ;
  mv = glm::lookAt(eye,center,up) ;
  glLoadMatrixf(&mv[0][0]) ;
  scene->draw(mv);
  glutSwapBuffers();
}

void reshape(int width, int height) {
  glm::mat4 mv; // for lookat
  float aspect = width / (float) height, zNear = 0.001, zFar = 99.99 ;
  float fovy = 45.0;
  glMatrixMode(GL_PROJECTION);
  mv = Transform::perspective(fovy,aspect,zNear,zFar);
  mv = glm::transpose(mv);
  glLoadMatrixf(&mv[0][0]);
  glViewport(0,0,width,height);
}


void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 'a':
    Transform::left(2, eye, up);
    break;
  case 'd':
    Transform::left(-2, eye, up);
    break;
  case 'w':
    Transform::up(2, eye, up);
    break;
  case 's':
    Transform::up(-2, eye, up);
    break;
  case 'q':
    exit(0);
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
  eye = glm::vec3(0,-5,0);
  center = glm::vec3(0,0,0);
  up = glm::vec3(0,0,1);
  Transform::up(30, eye, up);
}

int main (int argc, char* argv[]) {
  initVars();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow("CS194: Fluid Simulation");
  initShaderVars();
  scene = new Scene();
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable (GL_DEPTH_TEST);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  glutKeyboardUpFunc(keyUp);
  glutReshapeWindow(800,800);
  glutIdleFunc(idleFunc);
  glutPassiveMotionFunc(mouse);
  glutMainLoop();
  return 0;
}
