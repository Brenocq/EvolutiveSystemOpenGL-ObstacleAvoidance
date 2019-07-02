#ifndef ROBOT_H
#define ROBOT_H

class Robot{
public:
  Robot();
  ~Robot();

  void draw(void) const;
  void newOrientation(float _x, float _y, float _theta);// _x,_y: meters | _theta: degrees
  void newColor(int _color[3]);// {red, green, blue}
  void move(float linearVel, float anglarVel, float time);// linearVel:meters/second | angularVel:degrees/second | time:seconds
private:
  float x, y, theta;
  int color[3];
};


#endif // ROBOT_H
