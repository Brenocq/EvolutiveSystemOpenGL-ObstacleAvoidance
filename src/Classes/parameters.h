// Window Parameters
#define windowHeight 600
#define windowWidth 600
#define screenHeight 1080
#define screenWidth 1920
#define displaySteps 100000

// Evolutive system global parameters (fixed)
#define qtdRobots 5
#define qtdEnvironments 10
#define qtdRepetitions 10// Number of times that each environment will be tested to define the fitness
#define qtdObstacles 70
#define pointsCollision -50.0// Points per second in collision
#define pointsMoving 10.0// Points per second in maximum speed
#define maxPopulation 60// Number of populations tested with each environment
#define maxTime 600// Time that each population will be tested
#define envMutationRate 0.2

// OutputFile
#define printEverything 0 // 0->Print only essentials, 1->Print everything

// Debug
#define showAvFitness 0
#define showPopulationRobots 0
#define showPopulationEnvironments 1

// Fixed genes robots
// Set as -1 if you want to change
#define controlSideSensorActivation 0
#define controlFrontSensorActivation 0
#define controlLinearVelocity -1
#define controlMaximumRotation -1
#define controlSensorAngle -1

// Fixed genes environments
// Set as -1 if you want to change
#define controlFitnessMean 5
#define controlMutationRate 0.0
#define controlNeutralCrossing 0.0
#define controlNeutralMutation 0.5
#define controlBackMutationPrevention 1
#define controlCrossingCondition 2
