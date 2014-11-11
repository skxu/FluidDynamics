#include <GL/glut.h>
#include "Transform.h"
#include "table.h"

// Center of mass is at the top center of the table

Table::Table(float lX, float lY, float h) : Entity() {
  lenX = lX;
  lenY = lY;
  height = h;
  instantiateObjects();
}

void Table::instantiateObjects() {
  GLfloat ambient[4] = {0.3, 0.1, 0.1, 1.0};
  GLfloat diffuse[4] = {0.3, 0.1, 0.1, 1.0};
  GLfloat specular[4] = {0.1, 0.1, 0.1, 1.0};
  GLfloat shininess = 2;

  pushTransform();
    scale(lenX, lenY, height);

    translate(0,0,-0.002);

    pushTransform();
      translate(0,0,-0.05);
      scale(1,1,0.1);
      addObject(Entity::cube, ambient, diffuse, specular, shininess);
    popTransform();

    pushTransform();
      translate(0,0,-0.5125);
      pushTransform();
        translate(0.4,0.4,0);
        scale(0.05,0.05,0.975);
        addObject(Entity::cube, ambient, diffuse, specular, shininess);
      popTransform();
      pushTransform();
        translate(-0.4,0.4,0);
        scale(0.05,0.05,0.975);
        addObject(Entity::cube, ambient, diffuse, specular, shininess);
      popTransform();
      pushTransform();
        translate(-0.4,-0.4,0);
        scale(0.05,0.05,0.975);
        addObject(Entity::cube, ambient, diffuse, specular, shininess);
      popTransform();
      pushTransform();
        translate(0.4,-0.4,0);
        scale(0.05,0.05,0.975);
        addObject(Entity::cube, ambient, diffuse, specular, shininess);
      popTransform();
    popTransform();
  popTransform();
}