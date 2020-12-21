#ifndef MAIN_H
#define	MAIN_H

#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Particle.h"
#include "SnowObject.h"
#include "Grid.h"
#include "SimConstants.h"
#include "Shape.h"
#include "SnowSimulation.h"
#include "Scene.h"

using namespace std;

int frame_count;
vector<Shape*> snow_shapes;
SnowSimulation* snow_simulator;

void start(std::string scene);
void update();
void render();

// GLFW
GLFWwindow* initGLFWContext();
static void error_callback(int, const char*);

// GL
void initGLContext();

#endif

