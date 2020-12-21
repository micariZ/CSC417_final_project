
#include "SnowSimulation.h"

using namespace std;

SnowSimulation::SnowSimulation(Scene* scene) {
	frameCount = 0;
	
	snow = SnowObject::createShape(scene->snow_shapes);
	if (snow == NULL) return;

	Eigen::Vector2f origin(0, 0);
	grid = new Grid(origin, Eigen::Vector2f(WIN_METERS_X, WIN_METERS_Y), Eigen::Vector2f(256, 128), snow);
	grid->clear_nodes();
	grid->rasterize_particle_data();
	grid->compute_particle_volumes();
}

void SnowSimulation::update() {
	grid->clear_nodes();
	grid->rasterize_particle_data();
	grid->update_velocity();
	grid->handle_grid_collision();
	snow->update_def();
	grid->update_particle_velocity();
	grid->handle_particle_collision();
	snow->update_pos();
}
