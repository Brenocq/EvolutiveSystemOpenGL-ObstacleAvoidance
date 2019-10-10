// Window Parameters
#define windowHeight 800
#define windowWidth 800
#define screenHeight 1080
#define screenWidth 1920
#define displaySteps 1

// Evolutive system global parameters (fixed)
#define qtdRobots 10
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
#define controlSideSensorActivation -1
#define controlFrontSensorActivation -1
#define controlLinearVelocity -1
#define controlMaximumRotation -1
#define controlSensorAngle -1

// Fixed genes environments
// Set as -1 if you want to change
#define controlFitnessMean -1
#define controlMutationRate -1
#define controlNeutralCrossing -1
#define controlNeutralMutation -1
#define controlBackMutationPrevention -1
#define controlCrossingCondition -1
