#include "Particle.h"

Particle::Particle(Eigen::Vector2f pos, Eigen::Vector2f vel, float mass){
	position << pos;
	velocity << vel;
	d_velocity.setZero();
	this->mass = mass;
	F_elastic.setIdentity();
	F_plastic.setIdentity();
}

void Particle::compute_stress_based_force(Eigen::Matrix2f &stress_force) {
	float harden_parm = exp(HARDENING * (1 - F_plastic.determinant()));
	float Je = F_elastic.determinant();
	Eigen::Matrix2f U, V;
	Eigen::Vector2f S;
	Eigen::Matrix2f I;
    I.setIdentity();
	
	Eigen::JacobiSVD<Eigen::MatrixXf> svd(F_elastic, Eigen::ComputeFullU | Eigen::ComputeFullV);
	U = svd.matrixU();
	V = svd.matrixV();
	S = svd.singularValues();

    Eigen::Matrix2f step1 = 2 * MU * harden_parm * (F_elastic - U*V.transpose())*F_elastic.transpose();
    Eigen::Matrix2f step2 = LAMBDA * harden_parm * (Je - 1) * Je * I;

	stress_force = volume * (step1 + step2);
}

void Particle::update_deformation_gradient() {
	Eigen::Matrix2f I, F_next;
	Eigen::Matrix2f U, V;
	Eigen::Vector2f S;
	I.setIdentity();
	F_elastic = (I + TIMESTEP* d_velocity) * F_elastic;
	F_next = F_elastic * F_plastic;

	Eigen::JacobiSVD<Eigen::MatrixXf> svd(F_elastic, Eigen::ComputeFullU | Eigen::ComputeFullV);
	U = svd.matrixU();
	V = svd.matrixV();
	S = svd.singularValues();
	for (int i = 0; i < 2; i++) {
		if (S(i) < CRIT_COMPRESS)
			S(i) = CRIT_COMPRESS;
		else if (S(i) > CRIT_STRETCH)
			S(i) = CRIT_STRETCH;
	}

	F_elastic = U * S.asDiagonal() * V.transpose();
	F_plastic = F_elastic.inverse() * F_next;
}

void Particle::update_position() {
	//Simple euler integration
	position += TIMESTEP * velocity;
}