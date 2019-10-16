# Evolutive System OpenGL - Obstacle Avoidance

<p align="center">
<img src="https://github.com/Brenocq/EvolutiveSystemOpenGL-ObstacleAvoidance/blob/master/img/gifReleasev1.0.gif?raw=true" height="300">
</p>

Parallel evolution study of sensors and brain using OpenGL to simulate and Qt to generate graphs.

## Introduction
This project is the result of a scientific initiation with Professor Eduardo Simões (USP-ICMC) and is still under development. Initially, I started developing a simple GA for obstacle avoidance and exploration using 3 distance sensors as input, but the robots were evolving very quickly which was not interesting for analysis. In order to make the system more complex we are adding a neural network to the sensor analysis system. More info in the future.

## Installation
First, you need to download the OpenGl and Glut:
```
$ sudo apt-get install mesa-common-dev
$ sudo apt-get install freeglut3-dev

$ git clone https://github.com/Brenocq/EvolutiveSystemOpenGL-ObstacleAvoidance.git

```
## Running
When executing the program it is possible to choose in which file the data will be saved.
```
$ cd EvolutiveSystemOpenGL-ObstacleAvoidance/src
$ make
$ ./run <FILE_NAME>
```
You can also use `make run` to compile and execute at the same time.
## License
This project is licensed under the MIT License - see the [LICENSE.md](https://github.com/Brenocq/EvolutiveSystemOpenGL-ObstacleAvoidance/blob/master/LICENSE) file for details.
