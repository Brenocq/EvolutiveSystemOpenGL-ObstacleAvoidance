#include "environment.h"
#include <vector>

Environment::Environment(int _qtdRobots, int _qtdObstacles, int _maxTime, int _maxPopulation, int _pointsMoving, int _pointsCollision):
  qtdRobots(_qtdRobots), qtdObstacles(_qtdObstacles), currTime(0), maxTime(_maxTime), currentPopulation(0), maxPopulation(_maxPopulation),
  pointsMoving(_pointsMoving), pointsCollision(_pointsCollision)
{
    genes.resize(6,0);
    robotsMeanFitness.resize(qtdRobots);
    robotsFitness.resize(qtdRobots);
    robot.resize(qtdRobots);
    obstacle.resize(qtdObstacles);
}

void Environment::updateRobots(float time){
  if(currentPopulation<maxPopulation){
    vector<Object>objects;
    for (int i = 0; i < qtdRobots; i++) {
      objects.push_back(robot[i]);
    }
    for (int i = 0; i < numObstacles; i++) {
      objects.push_back(obstacle[i]);
    }
    for (int i = 0; i < qtdRobots; i++) {
      robot[i].move(objects, seconds);
    }
    currTime+=0.200;
    if(currTime==maxTime){
      currentPopulation++;
      currTime=0;
      if(currentPopulation<maxPopulation){
        newPopulationRobots();
        newRandomPositions();
      }
    }
  }
}

void Environment::initiateRobots(){
  //----- Set random genes -----//
  float genes[6];
  for (int i = 0; i < qtdRobots; i++){
    // SideSensorActivation   (0-3)meters
    genes[0] = (rand()%300)/100.0;
    // FrontSensorActivation  (0-3)meters
    genes[1] = (rand()%300)/100.0;
    // LinearVelocity         (0-1)meters/second
    genes[2] = (rand()%100)/100.0;
    // MaximumRotation        (0-10)degrees
    genes[3] = (rand()%1000)/100.0;
    // SensorAngle            (0-90)degrees
    genes[4] = (rand()%9000)/100.0;
    // Set random genes
    robot[i].newGene(genes);
    robot[i].setPoints(pointsCollision, pointsMoving);
    robot[i].fitness.push_back(0);
  }

  //----- new positions -----//
  newRandomPositions();// TODO
}

void Environment::newRandomPositions(){
  float posX, posY, angle;

  //----- Random positions obstacles -----//
  for (int i = 0; i < numObstacles; i++) {
    posX = (rand()%100 - 50)/5.0;// Some value between -10 and +10
    posY = (rand()%100 - 50)/5.0;// Some value between -10 and +10
    obstacle[i].setPosition(posX, posY);
  }

  //----- Random positions robots -----//
  for (int i = 0; i < qtdRobots; i++){
    robot[i].setId(i);

    posX = (rand()%180 - 90)/10.0;// Some value between -9 and +9
    posY = (rand()%180 - 90)/10.0;// Some value between -9 and +9
    angle = rand()%360;
    // Random position until reach a valid position for a robot
    bool validposition=false;
    while(!validposition){
      validposition=true;
      robot[i].newOrientation(posX, posY, angle);
      for (int j = 0; j < qtdRobots; j++){
        if(i!=j){
          if(distanceTwoObjects( &robot[i],&robot[j] ) <= (robot[i].getRadius()+robot[j].getRadius())){
            validposition=false;
            posX = (rand()%100 - 50)/5.0;// Some value between -10 and +10
            posY = (rand()%100 - 50)/5.0;// Some value between -10 and +10
          }
        }
      }
      for (int j = 0; j < numObstacles; j++) {
        if(distanceTwoObjects( &robot[i],&obstacle[j] ) <= (robot[i].getRadius()+obstacle[j].getRadius())){
          validposition=false;
          posX = (rand()%100 - 50)/5.0;// Some value between -10 and +10
          posY = (rand()%100 - 50)/5.0;// Some value between -10 and +10
        }
      }
    }
  }
}

void Environment::newPopulationRobots(){
  int bestRobot;
  vector< pair <int,int> >bestRobots;//first:fitness second:robotNumber

  //----- Calculate Average Fitness for each Robot -----//
  for(int i = 0; i < qtdRobots; i++){
    robot[i].updateMeanFitness(int(genes[0]));
  }
  //----- Select Best Robot -----//
  // The best one has the greater meanFitness
  for (int i = 0; i < qtdRobots; i++){
    bestRobots.push_back(make_pair(robot[i].meanFitness.back(),i));
  }
  sort(bestRobots.rbegin(),bestRobots.rend());

  bestRobot = bestRobots[0].second;
  bestRobotFitness = bestRobots[0].first;

  //----- Print population data -----//
  /*if(showPopulationRobots){
    cout<<endl<<"Fitness Population "<<populationNum<<":"<<endl;
    cout<<"\tMaximum Fitness: "<<populationTestTime*pointsMoving<<endl;
    cout<<"\tMinimum Fitness: "<<populationTestTime*pointsCollision<<endl;
    cout<<"\tAverage Fitness: "<<populationMeanFitness<<endl;
    cout<<"\tRobot "<<bestRobot<<" was the best robot with mean fitness equal to "<<bestRobotFitness
        <<" and fitness equal to "<<robot[bestRobot].fitness.back()<<endl<<endl;
  }*/
  //----- Crossing -----//
  bool condition;
  for (int i = 0; i < qtdRobots; i++) {
    // Select how to cross
    switch(int(gene[5])){
      case 0:
        condition = robot[i].fitness.back() < robot[bestRobot].fitness.back();
      break;
      case 1:
        condition = robot[i].fitness.back() < robot[bestRobot].meanFitness.back();
      break;
      case 2:
        condition = robot[i].meanFitness.back() < robot[bestRobot].meanFitness.back();
      break;
    }

    if(condition){
      for (int j = 0; j < int(robot[0].genes.size()); j++) {
        robot[i].genes[j] = float(neutralCrossing)*robot[i].genes[j]+(1-float(neutralCrossing))*robot[bestRobot].genes[j];
      }
    }
  }
  //----- Mutation -----//
  for (int i = 0; i < qtdRobots; i++) {
    robot[i].setColor(0,0,0);

    switch(crossingCondition){
      case 0:
        condition = robot[i].fitness.back() < robot[bestRobot].fitness.back();
      break;
      case 1:
        condition = robot[i].fitness.back() < averageFitness[bestRobot];
      break;
      case 2:
        condition = averageFitness[i] < averageFitness[bestRobot];
      break;
    }
    if(condition){
      // If will not use BackMutationPrevention
      int sumBMP=0;
      for (int j = 0; j < int(robot[0].mutatedGenes.size()); j++) {
        sumBMP+=robot[i].mutatedGenes[j];
      }
      if(!int(gene[4]) || sumBMP==int(robot[0].mutatedGenes.size())){
        // Set all genes as 0 to backMutationPrevention
        for (int j = 0; j < int(robot[0].mutatedGenes.size()); j++) {
          robot[i].mutatedGenes[j]=0;
        }
      }
      // Create new cromossome
      vector<float>genes(robot[0].genes.size());
      for (int j = 0; j < int(genes.size()); j++) {
        genes[j] = robot[i].genes[j];
      }

      for (int j = 0; j < int(genes.size()); j++) {
      int chanceMutation = rand()%100;
        if(chanceMutation < mutationRate*100 && robot[i].mutatedGenes[j]==0){
          robot[i].setColor(0.5,0.5,0);
          robot[i].mutatedGenes[j] = 1;
          switch(j){
            case 0:
            // SideSensorActivation   (0-3)meters
            genes[0] = float(neutralMutation)*genes[0] + (1-float(neutralMutation))*(rand()%300)/100.0;
            break;
            case 1:
            // FrontSensorActivation  (0-3)meters
            genes[1] = float(neutralMutation)*genes[1] + (1-float(neutralMutation))*(rand()%300)/100.0;
            break;
            case 2:
            // LinearVelocity         (0-1)meters/second
            genes[2] = float(neutralMutation)*genes[2] + (1-float(neutralMutation))*(rand()%100)/100.0;
            break;
            case 3:
            // MaximumRotation        (0-10)degrees
            genes[3] = float(neutralMutation)*genes[3] + (1-float(neutralMutation))*(rand()%1000)/100.0;
            break;
            case 4:
            // SensorAngle            (0-90)degrees
            genes[4] = float(neutralMutation)*genes[4] + (1-float(neutralMutation))*(rand()%9000)/100.0;
            break;
          }
        }
      }
      robot[i].newGene(genes);
    }
  }
  robot[bestRobot].setColor(0,0,1);

  //----- New population fitness -----//
  for (int i = 0; i < qtdRobots; i++) {
    robot[i].fitness.push_back(0);
  }
}
