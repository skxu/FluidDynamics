#include <GL/glut.h>
#include "Transform.h"
#include "container.h"

// Center of mass is at the bottom center of the container

Container::Container(float lX, float lY, float h) : Entity() {
  lenX = lX;
  lenY = lY;
  height = h;
  depthZ = 0.02;
  instantiateObjects();
}

void Container::instantiateObjects() {
  GLfloat frame_ambient[4] = {0.1, 0.1, 0.1, 1.0};
  GLfloat frame_diffuse[4] = {0.1, 0.1, 0.1, 1.0};
  GLfloat frame_specular[4] = {0.0, 0.0, 0.0, 1.0};
  GLfloat frame_shininess = 2;

  // Construct the frame first
  pushTransform();
    translate(0,lenY/2,depthZ/2);
    scale(lenX+depthZ/2,depthZ,depthZ);
    addObject(Entity::cube, frame_ambient, frame_diffuse, frame_specular, frame_shininess);
  popTransform();
  pushTransform();
    translate(0,-lenY/2,depthZ/2);
    scale(lenX+depthZ/2,depthZ,depthZ);
    addObject(Entity::cube, frame_ambient, frame_diffuse, frame_specular, frame_shininess);
  popTransform();
  pushTransform();
    translate(lenX/2,0,depthZ/2);
    scale(depthZ,lenY+depthZ/2,depthZ);
    addObject(Entity::cube, frame_ambient, frame_diffuse, frame_specular, frame_shininess);
  popTransform();
  pushTransform();
    translate(-lenX/2,0,depthZ/2);
    scale(depthZ,lenY+depthZ/2,depthZ);
    addObject(Entity::cube, frame_ambient, frame_diffuse, frame_specular, frame_shininess);
  popTransform();
  pushTransform();
    translate(0,lenY/2,depthZ/2+height);
    scale(lenX+depthZ/2,depthZ,depthZ);
    addObject(Entity::cube, frame_ambient, frame_diffuse, frame_specular, frame_shininess);
  popTransform();
  pushTransform();
    translate(0,-lenY/2,depthZ/2+height);
    scale(lenX+depthZ/2,depthZ,depthZ);
    addObject(Entity::cube, frame_ambient, frame_diffuse, frame_specular, frame_shininess);
  popTransform();
  pushTransform();
    translate(lenX/2,0,depthZ/2+height);
    scale(depthZ,lenY+depthZ/2,depthZ);
    addObject(Entity::cube, frame_ambient, frame_diffuse, frame_specular, frame_shininess);
  popTransform();
  pushTransform();
    translate(-lenX/2,0,depthZ/2+height);
    scale(depthZ,lenY+depthZ/2,depthZ);
    addObject(Entity::cube, frame_ambient, frame_diffuse, frame_specular, frame_shininess);
  popTransform();
  pushTransform();
    translate(lenX/2,lenY/2,depthZ/2+height/2);
    scale(depthZ,depthZ,height+depthZ/2);
    addObject(Entity::cube, frame_ambient, frame_diffuse, frame_specular, frame_shininess);
  popTransform();
  pushTransform();
    translate(lenX/2,-lenY/2,depthZ/2+height/2);
    scale(depthZ,depthZ,height+depthZ/2);
    addObject(Entity::cube, frame_ambient, frame_diffuse, frame_specular, frame_shininess);
  popTransform();
  pushTransform();
    translate(-lenX/2,-lenY/2,depthZ/2+height/2);
    scale(depthZ,depthZ,height+depthZ/2);
    addObject(Entity::cube, frame_ambient, frame_diffuse, frame_specular, frame_shininess);
  popTransform();
  pushTransform();
    translate(-lenX/2,lenY/2,depthZ/2+height/2);
    scale(depthZ,depthZ,height+depthZ/2);
    addObject(Entity::cube, frame_ambient, frame_diffuse, frame_specular, frame_shininess);
  popTransform();

  GLfloat glass_ambient[4] = {0.0, 0.0, 0.0, 0.15};
  GLfloat glass_diffuse[4] = {0.6, 0.7, 0.7, 0.0};
  GLfloat glass_specular[4] = {0.9, 0.9, 0.9, 0.0};
  GLfloat glass_shininess = 96.0;
  pushTransform();
    translate(0,0,depthZ/2);
    scale(lenX,lenY,depthZ);
    addObject(Entity::cube, glass_ambient, glass_diffuse, glass_specular, glass_shininess);
  popTransform();
  pushTransform();
    translate(0,lenY/2,height/2+depthZ/2);
    scale(lenX,depthZ,height);
    addObject(Entity::cube, glass_ambient, glass_diffuse, glass_specular, glass_shininess);
  popTransform();
  pushTransform();
    translate(0,-lenY/2,height/2+depthZ/2);
    scale(lenX,depthZ,height);
    addObject(Entity::cube, glass_ambient, glass_diffuse, glass_specular, glass_shininess);
  popTransform();
  pushTransform();
    translate(lenX/2,0,height/2+depthZ/2);
    scale(depthZ,lenY,height);
    addObject(Entity::cube, glass_ambient, glass_diffuse, glass_specular, glass_shininess);
  popTransform();
  pushTransform();
    translate(-lenX/2,0,height/2+depthZ/2);
    scale(depthZ,lenY,height);
    addObject(Entity::cube, glass_ambient, glass_diffuse, glass_specular, glass_shininess);
  popTransform();
}