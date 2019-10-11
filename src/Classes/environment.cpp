#include "environment.h"
#include "quadtree.h"
#include <vector>
#include <math.h>

Environment::Environment():
  finished(false), currTime(0), currentPopulation(0)
{
  srand(time(0));

  robot.resize(qtdRobots);
  obstacle.resize(qtdObstacles);
  genes.resize(6,0);
  fitness.push_back(0);

  initiateRobots();
}

void Environment::updateRobots(float time){
  if(finished==true){
    finished=false;
    currTime=0;
    currentPopulation=0;
    fitness.push_back(0);
    initiateRobots();
  }
  if(currentPopulation<maxPopulation){
    vector<Object*>objects;
    for (int i = 0; i < qtdRobots; i++) {
      objects.push_back(&robot[i]);
    }
    for (int i = 0; i < qtdObstacles; i++){
      objects.push_back(&obstacle[i]);
    }

    for (int i = 0; i < qtdRobots; i++){
      robot[i].move(objects, time);
    }
    currTime+=0.200;
    if(currTime>=maxTime){
      currentPopulation++;
      currTime=0;
      if(currentPopulation<maxPopulation){
        newPopulationRobots();
        newRandomPositions();
      }
    }
  }else{
    finished = true;
  }
}

void Environment::initiateRobots(){
  //----- Set random genes -----//
  for (int i = 0; i < qtdRobots; i++){
    vector<float> genesAnatomy;
    vector<float> genesBrain;
    int qtdSensors = 0;

    // MaximumVelocity         (0-1) meters/second
    if(controlMaximumVelocity==-1) genesAnatomy.push_back( float(rand()%100)/100.0 );
    else                           genesAnatomy.push_back( controlMaximumVelocity );
    // MaximumRotation        (0-10) degrees
    if(controlMaximumRotation==-1) genesAnatomy.push_back( float(rand()%1000)/100.0 );
    else                           genesAnatomy.push_back( controlMaximumRotation );
    // Qtd infrared sensors   (0-10) sensors
    int qtdIR=0;
    if(controlQtdIR==-1) qtdIR = rand()%11;
    else                 qtdIR = controlQtdIR;
    qtdSensors += qtdIR;

    genesAnatomy.push_back(qtdIR);
    //--- Create infrared Sensors ---//
    for(int j=0;j<qtdIR;j++){
      // For each sensor will be defined angle and maxDist
      // Angle              (0-360) degrees
      genesAnatomy.push_back( float(rand()%(360*100))/100 );
      // MaxDist            (0-5) meters
      genesAnatomy.push_back( float(rand()%(5*100))/100 );
    }

    //--- Create Brain ---//
    for (int j = 0; j < 2*qtdSensors; j++) {
      // Will create a weight to each sensor (-10,10) to calculate the velocity
      // Will create a weight to each sensor (-10,10) to calculate the rotation
      genesBrain.push_back( float(rand()%(2*1*100))/100 -1);
    }

    //--- Set random genes ---//
    robot[i].newGenes(genesAnatomy, genesBrain);
    robot[i].fitness.push_back(0);
  }
  //----- New positions -----//
  newRandomPositions();
}

void Environment::newRandomPositions(){
  float posX, posY, angle;

  //----- Random positions obstacles -----//
  for (int i = 0; i < qtdObstacles; i++) {
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
      for (int j = 0; j < qtdObstacles; j++) {
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
  int bestRobot, bestRobotFitness;
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

  // The enviroment fitness is the best robot fitness in the end of the tests
  if(currentPopulation==maxPopulation-1){
    fitness.back()=bestRobotFitness;
  }

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
    switch(int(genes[5])){
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
      for (int j = 0; j < int(robot[0].genesAnatomy.size()); j++) {
        robot[i].genesAnatomy[j] = float(genes[2])*robot[i].genesAnatomy[j]+(1-float(genes[2]))*robot[bestRobot].genesAnatomy[j];
      }
      for (int j = 0; j < int(robot[0].genesBrain.size()); j++) {
        robot[i].genesBrain[j] = float(genes[2])*robot[i].genesBrain[j]+(1-float(genes[2]))*robot[bestRobot].genesBrain[j];
      }
    }
  }
  //----- Mutation -----//
  /*for (int i = 0; i < qtdRobots; i++) {
    robot[i].setColor(0,0,0);

    switch(int(genes[5])){
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
      // Create new cromossome
      vector<float>mutatedGenesAnatomy;
      mutatedGenesAnatomy = robot[i].genesAnatomy;

      vector<float>mutatedGenesBrain;
      mutatedGenesBrain = robot[i].genesBrain;

      for (int j = 0; j < int(mutatedGenesAnatomy.size()); j++) {
        int chanceMutation = rand()%100;
        if(chanceMutation < genes[1]*100){
          robot[i].setColor(0.5,0.5,0);
          switch(j){
            case 0:
            // SideSensorActivation   (0-3)meters
            if(controlSideSensorActivation==-1)
              genesRobot[0] = float(genes[2])*genesRobot[0] + (1-float(genes[2]))*(rand()%300)/100.0;
            break;
            case 1:
            // FrontSensorActivation  (0-3)meters
            if(controlFrontSensorActivation==-1)
              genesRobot[1] = float(genes[2])*genesRobot[1] + (1-float(genes[2]))*(rand()%300)/100.0;
            break;
            case 2:
            // LinearVelocity         (0-1)meters/second
            if(controlLinearVelocity==-1)
              genesRobot[2] = float(genes[2])*genesRobot[2] + (1-float(genes[2]))*(rand()%100)/100.0;
            break;
            case 3:
            // MaximumRotation        (0-10)degrees
            if(controlMaximumRotation==-1)
              genesRobot[3] = float(genes[2])*genesRobot[3] + (1-float(genes[2]))*(rand()%1000)/100.0;
            break;
            case 4:
            // SensorAngle            (0-90)degrees
            if(controlSensorAngle==-1)
              genesRobot[4] = float(genes[2])*genesRobot[4] + (1-float(genes[2]))*(rand()%9000)/100.0;
            break;
          }
        }
      }
      robot[i].newGene(genesRobot);
    }
  }*/
  robot[bestRobot].setColor(0,0,1);

  //----- New population fitness -----//
  for (int i = 0; i < qtdRobots; i++) {
    robot[i].fitness.push_back(0);
  }
}

void Environment::draw(){
  // Progress bar
  glColor3f(1, 0, 0);
  glBegin(GL_POLYGON);
  glVertex2d(-1,1);
  glVertex2d((float(currentPopulation)/maxPopulation+currTime/(maxPopulation*maxTime))*2-1,1);
  glVertex2d((float(currentPopulation)/maxPopulation+currTime/(maxPopulation*maxTime))*2-1,0.99);
  glVertex2d(-1,0.99);
  glEnd();

  for (int i = 0; i < qtdRobots; i++) {
    robot[i].draw();
  }
  for (int i = 0; i < qtdObstacles; i++) {
    obstacle[i].draw();
  }

  // Draw QuadTree
  vector<Object>objects;
  for (int i = 0; i < qtdRobots; i++) {
    objects.push_back(robot[i]);
  }
  for (int i = 0; i < qtdObstacles; i++){
    objects.push_back(obstacle[i]);
  }
}

bool Environment::getFinished() const{
  return finished;
}
