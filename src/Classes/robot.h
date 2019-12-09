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
	uint8_t getImmunity() const;
  //----- Setters -----//
  void newGenes(vector<float> _genesAnatomy, vector<float> _genesBrain);
  void newOrientation(float _x, float _y, float _theta);// _x,_y: meters | _theta: degrees
  void setTheta(float _theta);
  void setHeadColor(int r, int g, int b);
	void setImmunity(uint8_t _immunity);

  vector<float> fitness;
  vector<float> meanFitness;
  vector<float> genesAnatomy;
  // Genes:
  // 0-MaximumVelocity        (0-1)meters/second
  // 1-MaximumRotation        (0-10)degrees
  // [2..6]-SensorActivated   (0-10)sensors
  vector<float> genesBrain;
  // neural network

private:
  float theta;
  float lastX, lastY;
  float timeLastXY;
  vector<int> headColor;
  bool inCollision;
  vector<char> sensorValues;// 4 Possible values: Near / Middle / Far / Too Far
  // Calculatio fitness with area explored
  //float minX, maxX, minY, maxY;
  //float lastFitnessExploration;
	uint8_t immunity; 
};


#endif // ROBOT_H
