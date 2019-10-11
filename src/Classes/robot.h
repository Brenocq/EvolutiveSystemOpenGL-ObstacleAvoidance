#ifndef ROBOT_H
#define ROBOT_H
#include "object.h"
#include "parameters.h"
#include "quadtree.h"
#include <vector>
using namespace std;

class Robot: public Object{
public:
  Robot();
  ~Robot();

  void draw(void) const;
  void move(vector<Object*> objects, float seconds);
  void rotate(float angle);
  void updateSensor(vector<Object*> objects);
  void updateMeanFitness(int sizeMean);

  //----- Getters -----//
  float getTheta() const;
  //----- Setters -----//
  void newGenes(vector<float> _genesAnatomy, vector<float> _genesBrain);
  void newOrientation(float _x, float _y, float _theta);// _x,_y: meters | _theta: degrees
  void setTheta(float _theta);

  vector<float> fitness;
  vector<float> meanFitness;
  vector<float> genesAnatomy;
  // Genes:
  // 0-MaximumVelocity        (0-1)meters/second
  // 1-MaximumRotation        (0-10)degrees
  // 2-QtdIRsensors           (0-10)sensors
  // ...-SensorsInfo
  vector<float> genesBrain;
  // size = 2*qtdSensors

private:
  float theta;
  float lastX, lastY;
  float timeLastXY;
  bool inCollision;
  vector<float> sensorValues;
};


#endif // ROBOT_H
