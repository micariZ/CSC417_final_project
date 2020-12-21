#ifndef GRID_H
#define	GRID_H

#include <math.h>
#include "SnowObject.h"
#include "SimConstants.h"
#include "B-spline.h"
#include <iostream>
#include <Eigen/Dense>
#include <EigenTypes.h>

const float BSPLINE_EPSILON = 1e-4;
const int BSPLINE_RADIUS = 2;

//Grid node data
typedef struct GridNode {
	float mass;
	Eigen::Vector2f position, velocity, force, next_velocity;

} GridNode;

class Grid {
private:
	void transfer_mass();
	void transfer_velocity();
	void get_gird_pos(Eigen::Vector2f &pos, Eigen::Vector2f partical_pos);
	void compute_internal_force();
	void handle_collision(Eigen::Vector2f& position, Eigen::Vector2f& velocity);
	void get_node_pos(Eigen::Vector2f& p, int i, int j);
	
public:
	Eigen::Vector2f origin, size, cellsize;
	SnowObject* obj;
	int nodes_length;
	GridNode* nodes;

	Grid(Eigen::Vector2f pos, Eigen::Vector2f dims, Eigen::Vector2f cells, SnowObject* obj);
	void compute_particle_volumes();
	void rasterize_particle_data();
	void update_velocity();
	void clear_nodes();
	void handle_particle_collision();
	void handle_grid_collision();
	void update_particle_velocity();

	void draw();
};

#endif