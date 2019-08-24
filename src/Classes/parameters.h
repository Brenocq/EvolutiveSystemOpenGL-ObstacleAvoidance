// Window Parameters
#define windowHeight 600
#define windowWidth 600
#define screenHeight 1080
#define screenWidth 1920
#define displaySteps 1

// Evolutive system global parameters (fixed)
#define qtdRobots 20
#define qtdEnvironments 10
#define qtdRepetitions 10// Number of times that each environment will be tested to define the fitness
#define qtdObstacles 50
#define pointsCollision -50.0// Points per second in collision
#define pointsMoving 10.0// Points per second in maximum speed
#define maxPopulation 50// Number of populations tested with each environment
#define maxTime 200// Time that each population will be tested
#define envMutationRate 0.2

// Debug
#define showAvFitness 0
#define showPopulationRobots 0
#define showPopulationEnvironments 1