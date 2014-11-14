#include <algorithm>
#include <GLUT/glut.h>
#include <glm/glm.hpp>
#include <sys/time.h>
#include "shaders.h"
#include "Transform.h"
#include "scene.h"

using namespace std;

// Main variables in the program.  
#define MAINPROGRAM 
#include "variables.h"

// Global variables are needed to synchronize across OpenGL functions
Scene* scene;
int timeidx;
struct timeval starttv, endtv;
const int FPS = 20;
bool pauseanim;

void updatetimeidx();
void display();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void idleFunc();
void keyUp (unsigned char key, int x, int y);
void mouse(int x, int y);
void initShaderVars();
void initVars();
double getSecondsFromTimeVal(timeval & t);


void display() {
  glClearColor(0,0,0.2,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glm::mat4 mv ;
  mv = glm::lookAt(eye,center,up) ;
  glLoadMatrixf(&mv[0][0]) ;
  scene->draw(mv, timeidx);
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
  case 'p':
    pauseanim = !pauseanim;
    break;
  case 'r':
    timeidx = 0;
    break;
  case 'n':
    timeidx = max(0, timeidx -1);
    break;
  case 'm':
    timeidx = min(timeidx+1, scene->numTimeSteps()-1);
    break;
  case 'q':
    scene->destroy();
    exit(0);
  }
}

void idleFunc ( ) {
  updatetimeidx();
  glutPostRedisplay();
}

void updatetimeidx() {
  if (!pauseanim) {
    gettimeofday(&endtv, NULL);
    double start_time_s = getSecondsFromTimeVal(starttv);
    double diff_time_s = getSecondsFromTimeVal(endtv) - start_time_s;
    if (diff_time_s >= (1.0/FPS)) {
      timeidx = min(timeidx+1, scene->numTimeSteps()-1);
      gettimeofday(&starttv, NULL);
    }
  }
}

double getSecondsFromTimeVal(timeval & t) {
  return t.tv_sec + t.tv_usec/1000000.0;
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
  eye = glm::vec3(0,-3,0);
  center = glm::vec3(0,0,0);
  up = glm::vec3(0,0,1);
  Transform::up(45, eye, up);
  timeidx = 0;
  gettimeofday(&starttv, NULL);
  pauseanim = false;
}

int main (int argc, char* argv[]) {
  initVars();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow("CS194: Fluid Simulation");
  initShaderVars();
  scene = new Scene("../inputs/data-test.txt");
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
