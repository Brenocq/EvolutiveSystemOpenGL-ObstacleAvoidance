#include "environment.h"
#include "quadtree.h"
#include <vector>

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
  float genes[6];
  for (int i = 0; i < qtdRobots; i++){
    // SideSensorActivation   (0-3)meters
    if(controlSideSensorActivation==-1)
      genes[0] = (rand()%300)/100.0;
    else
      genes[0] = controlSideSensorActivation;

    // FrontSensorActivation  (0-3)meters
    if(controlFrontSensorActivation==-1)
      genes[1] = (rand()%300)/100.0;
    else
      genes[1] = controlFrontSensorActivation;

    // LinearVelocity         (0-1)meters/second
    if(controlLinearVelocity==-1)
      genes[2] = (rand()%100)/100.0;
    else
      genes[2] = controlLinearVelocity;

    // MaximumRotation        (0-10)degrees
    if(controlMaximumRotation==-1)
      genes[3] = (rand()%1000)/100.0;
    else
      genes[3] = controlMaximumRotation;

    // SensorAngle            (0-90)degrees
    if(controlSensorAngle==-1)
      genes[4] = (rand()%9000)/100.0;
    else
      genes[4] = controlSensorAngle;


    // Set random genes
    robot[i].newGene(genes);
    robot[i].fitness.push_back(0);
  }
  //----- new positions -----//
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
      for (int j = 0; j < int(robot[0].genes.size()); j++) {
        robot[i].genes[j] = float(genes[2])*robot[i].genes[j]+(1-float(genes[2]))*robot[bestRobot].genes[j];
      }
    }
  }
  //----- Mutation -----//
  for (int i = 0; i < qtdRobots; i++) {
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
      // If will not use BackMutationPrevention
      int sumBMP=0;
      for (int j = 0; j < int(robot[0].mutatedGenes.size()); j++) {
        sumBMP+=robot[i].mutatedGenes[j];
      }
      if(!int(genes[4]) || sumBMP==int(robot[0].mutatedGenes.size())){
        // Set all genes as 0 to backMutationPrevention
        for (int j = 0; j < int(robot[0].mutatedGenes.size()); j++) {
          robot[i].mutatedGenes[j]=0;
        }
      }
      // Create new cromossome
      vector<float>genesRobot(robot[0].genes.size());
      for (int j = 0; j < int(genes.size()); j++) {
        genesRobot[j] = robot[i].genes[j];
      }
      for (int j = 0; j < int(genes.size()); j++) {
      int chanceMutation = rand()%100;
        if(chanceMutation < genes[1]*100 && robot[i].mutatedGenes[j]==0){
          robot[i].setColor(0.5,0.5,0);
          robot[i].mutatedGenes[j] = 1;
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
  }
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

  /*QuadTree *qTree = new QuadTree(0,0,20,20,1);
  for(auto object : objects){
    glColor3f(1, 1, 0);
    glBegin(GL_POLYGON);
    glVertex2d(object.getX()/10+0.005, object.getY()/10+0.005);
    glVertex2d(object.getX()/10+0.005, object.getY()/10-0.005);
    glVertex2d(object.getX()/10-0.005, object.getY()/10-0.005);
    glVertex2d(object.getX()/10-0.005, object.getY()/10+0.005);
    glEnd();
    qTree->insert(&object);
  }
  qTree->draw();
  delete qTree;*/
}

bool Environment::getFinished() const{
  return finished;
}
