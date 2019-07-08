#ifndef OBSTACLE_H
#define OBSTACLE_H
#include <vector>
#include "object.h"

using namespace std;

class Obstacle: public Object{
public:
  Obstacle();
  ~Obstacle();

  //----- Setters -----//
  void setPosition(float _x, float _y);// _x,_y: meters

  void draw(void) const;
private:
};

#endif // OBSTACLE_H
