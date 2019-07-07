#ifndef ROBOT_H
#define ROBOT_H

#include <vector>
using namespace std;

class Robot{
public:
  Robot();
  ~Robot();

  //----- Getters -----//
  float getX() const;
  float getY() const;
  float getTheta() const;
  float getRadius() const;
  int getId() const;
  //----- Setters -----//
  void newGene(float _genes[6]);
  void newColor(float r, float g, float b);// (red, green, blue)
  void newOrientation(float _x, float _y, float _theta);// _x,_y: meters | _theta: degrees
  void setId(int _id);
  void setTheta(float _theta);

  void draw(void) const;
  void move(vector<Robot> robots, float seconds);
  void rotate(float angle);
  void updateSensor(vector<Robot> robots);

private:
  float genes[6];
  // 0-SideSensorActivation   (0-3)meters
  // 1-FrontSensorActivation  (0-3)meters
  // 2-LinearVelocity         (0-1)meters/second
  // 3-MaximumRotation        (0-10)degrees
  // 4-SensorAngle            (0-90)degrees
  float fitness;
  float x, y, theta, radius;
  float color[3];
  unsigned int id;
  bool inCollision;
  float sensorValues[3];// Left, Front, Right
};


#endif // ROBOT_H
