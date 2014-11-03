#ifndef __INCLUDECONTAINER
#define __INCLUDECONTAINER

#include "entity.h"

class Container : public Entity { 
  public:
    Container(float lX, float lY, float h);
    void instantiateObjects();
  private:
    float lenX;
    float lenY;
    float height;
    float depthZ;
};


#endif // __INCLUDECONTAINER