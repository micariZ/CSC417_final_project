#ifndef SNOWSIM_H
#define	SNOWSIM_H

#include "Grid.h"
#include <stdlib.h>
#include "Particle.h"
#include "SnowObject.h"
#include "Grid.h"
#include "SimConstants.h"
#include "Shape.h"
#include "Scene.h"

class SnowSimulation {
public:
	int frameCount;
	Grid* grid;
	SnowObject* snow;

	SnowSimulation(Scene* scene);
	void update();
};


#endif