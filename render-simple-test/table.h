#ifndef __INCLUDETABLE
#define __INCLUDETABLE

#include "entity.h"

class Table : public Entity { 
  public:
    Table(float lX, float lY, float h);
    void instantiateObjects();
  private:
    float lenX;
    float lenY;
    float height;
};


#endif // __INCLUDETABLE