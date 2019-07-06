#ifndef ROBOT_H
#define ROBOT_H

class Robot{
public:
  Robot();
  ~Robot();

  //----- Getters -----//
  float getX() const;
  float getY() const;
  float getTheta() const;
  float getRadius() const;

  void draw(void) const;
  void newOrientation(float _x, float _y, float _theta);// _x,_y: meters | _theta: degrees
  void newColor(float r, float g, float b);// (red, green, blue)
  void move(float linearVel, float anglarVel, float seconds);// linearVel:meters/second | angularVel:degrees/second | time:seconds

private:
  float genes[6];
  // RightSensorActivation, LeftSensorActivation, FrontSensorActivation, LinearVelocity, MaximumRotation, SensorAngle
  float fitness;
  float x, y, theta, radius;
  float color[3];
};


#endif // ROBOT_H
