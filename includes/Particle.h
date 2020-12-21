#ifndef PARTICLE_H
#define	PARTICLE_H

#include <cmath>
#include "SimConstants.h"
#include <Eigen/Dense>
#include <EigenTypes.h>
#include <iostream>

class Particle {
public:
	float volume, mass, density;
	Eigen::Vector2f position, velocity;
	Eigen::Matrix2f F_elastic, F_plastic, d_velocity;
	Eigen::Vector2f d_weight[NUM_NODE_COVERS];
	float weights[NUM_NODE_COVERS];

	Particle(Eigen::Vector2f pos, Eigen::Vector2f vel, float mass);
	void compute_stress_based_force(Eigen::Matrix2f& stress_force);
	void update_position();
	void svd_helper(Eigen::Matrix2f data, Eigen::Matrix2f &w, Eigen::Vector2f& e, Eigen::Matrix2f& v);
	void update_deformation_gradient();
};

#endif

