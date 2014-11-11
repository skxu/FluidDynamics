#include <iostream>
#include <GL/glut.h>
#include "Transform.h"
#include "entity.h"
#include "variables.h"

using namespace std;

Entity::Entity() {
  elements = vector<object>();
  s = stack<glm::mat4>();
  s.push(glm::mat4(1.0));
}

void Entity::rightmultiply(const glm::mat4 & M) {
    glm::mat4 &T = s.top() ;
    // Right multiply M, but do this left to account for row/column major
    T = T*M  ;
}

void Entity::pushTransform() {
  s.push(s.top());
}

void Entity::popTransform() {
  if (s.size() <= 1) {
    cerr << "Stack has no elements. Cannot pop.\n" << endl;
  } else {
    s.pop();
  }
}

void Entity::translate(float x, float y, float z) {
  glm::mat4 curTransMat = glm::transpose(Transform::translate(x, y, z));
  rightmultiply(curTransMat);
}

void Entity::scale(float x, float y, float z) {
  glm::mat4 curScaleMat = glm::transpose(Transform::scale(x, y, z));
  rightmultiply(curScaleMat);
}

void Entity::rotate(float ax, float ay, float az, float amount_degrees) {
  glm::vec3 _axis = glm::vec3(ax, ay, az);
  glm::vec3 axis = glm::normalize(_axis);
  glm::mat4 curRotationMat = glm::mat4(Transform::rotate(amount_degrees, axis));
  curRotationMat = glm::transpose(curRotationMat);
  rightmultiply(curRotationMat);
}

void Entity::update() {
  // This should be overwritten if the objects in the entity move.
}

void Entity::draw(glm::mat4 &mv) {
  for(std::vector<object>::iterator obj = elements.begin(); obj != elements.end(); obj++) {
    glUniform4fv(ambientcol, 1, obj->ambient);
    glUniform4fv(diffusecol, 1, obj->diffuse);
    glUniform4fv(specularcol, 1, obj->specular);
    glUniform1i(shininesscol, obj->shininess);
    glLoadMatrixf(&(mv * obj->transform)[0][0]);
    if (obj->type == sphere) {
      glutSolidSphere(1.0, 10.0, 10.0);
    } else if (obj->type == cube) {
      glutSolidCube(1.0);
    }
  }
}

void Entity::addObject(Entity::shape type, GLfloat ambient[4], GLfloat diffuse[4], GLfloat specular[4], GLfloat shininess) {
  object * o = new object();
  o->type = type;
  for (int i = 0; i < 4; i++) {
    (o->ambient)[i] = ambient[i];
    (o->diffuse)[i] = diffuse[i];
    (o->specular)[i] = specular[i];
  }
  o->shininess = shininess;
  o->transform = s.top();
  elements.push_back(*o);
  
}
