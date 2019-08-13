#ifndef OBJECT_H
#define OBJECT_H
#include <vector>
using namespace std;

#define fitnessMean 3

class Object{
public:
  Object();
  ~Object();

  //----- Getters -----//
  float getX() const;
  float getY() const;
  float getRadius() const;
  int getId() const;
  //----- Setters -----//
  void setColor(float r, float g, float b);// (red, green, blue)
  void setId(int _id);
  virtual void draw(void) const;
protected:
  unsigned int id;
  float x, y, radius;
  float color[3];
};

#endif // OBJECT_H
