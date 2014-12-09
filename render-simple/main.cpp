#include <algorithm>
#include <ctype.h>
#include <GLUT/glut.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include "shaders.h"
#include "Transform.h"
#include "scene.h"

// Main variables in the program.  
#define MAINPROGRAM 
#include "variables.h"

// Global variables are needed to be accessible across OpenGL functions
Scene* scene;
int timeidx;
struct timeval starttv, endtv;
const int FPS = 20;
bool pauseanim;

// Prototype functions for main file
void updatetimeidx();
void display();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void idleFunc();
void keyUp (unsigned char key, int x, int y);
void mouse(int x, int y);
void initShaderVars();
void initVars();
void initScene(int argc, char* argv[]);
void printUsage();
double getSecondsFromTimeVal(timeval & t);

/* Display: tells OpenGL what to render. Will tell the scene to
 * draw all the objects in the scene. */
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

/* Reshape: reshapes the GLUT window of our program */
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

/* Keyboard commands */
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 'a':
    Transform::left(-2, eye, up);
    break;
  case 'd':
    Transform::left(2, eye, up);
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
    timeidx = std::max(0, timeidx -1);
    break;
  case 'm':
    timeidx = std::min(timeidx+1, scene->numTimeSteps()-1);
    break;
  case 'q':
    delete scene;
    exit(0);
  }
}

// Called indefinitely.
void idleFunc ( ) {
  updatetimeidx();
  glutPostRedisplay();
}

/* Update time index, which determines which time stamp to render
/* for our particles */
void updatetimeidx() {
  if (!pauseanim) {
    gettimeofday(&endtv, NULL);
    double start_time_s = getSecondsFromTimeVal(starttv);
    double diff_time_s = getSecondsFromTimeVal(endtv) - start_time_s;
    if (diff_time_s >= (1.0/FPS)) {
      timeidx = std::min(timeidx+1, scene->numTimeSteps()-1);
      gettimeofday(&starttv, NULL);
    }
  }
}

// Converts a timeval t to its seconds + useconds value.
double getSecondsFromTimeVal(timeval & t) {
  return t.tv_sec + t.tv_usec/1000000.0;
}

// Action that takes place when you remove finger from key.
void keyUp (unsigned char key, int x, int y) {

}

// Initialize shaders and gets references to uniform values
void initShaderVars() {
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

// Initializes all global variables
void initVars() {
  eye = glm::vec3(0,-2,0.2);
  center = glm::vec3(0,0,0.2);
  up = glm::vec3(0,0,1);
  Transform::up(40, eye, up);
  timeidx = 0;
  gettimeofday(&starttv, NULL);
  pauseanim = false;
}

// Initializes the scene with the specified input data from command line
void initScene(int argc, char* argv[]) {
  char * filename = NULL;
  int c;

  if (argc == 1) {
    std::cout << "[Error]: Must specify an input file. Usage is: " << std::endl;
    printUsage();
    exit(1);
  }

  while ((c = getopt (argc, argv, "f:")) != -1) {
    switch (c)
    {
      case 'f':
        filename = optarg;
        break;
    }
  }

  if (filename == NULL) {
    std::cout << "[Error]: Unable to read input file. Usage is: " << std::endl;
    printUsage();
    exit(1);
  }

  scene = new Scene(filename);
}

// Print usage of command line args
void printUsage() {
  std::cout << "   ./fluidsim" << std::endl;
  std::cout << "\t-f\tINPUT_FILENAME" << std::endl;
}

// Main function that calls OpenGL loop and sets callbacks
int main (int argc, char* argv[]) {
  initVars();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow("CS194: Fluid Simulation");
  initShaderVars();
  initScene(argc, argv);

  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable (GL_DEPTH_TEST);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  glutKeyboardUpFunc(keyUp);
  glutReshapeWindow(800,800);
  glutIdleFunc(idleFunc);
  glutMainLoop();
  return 0;
}
