#include "SnowObject.h"

SnowObject::SnowObject(int obj_size){
	size = obj_size;
	particles.reserve(size);
}

void SnowObject::update_def(){
	for (int i=0; i<size; i++){
		particles[i].update_deformation_gradient();
	}
}

void SnowObject::update_pos() {
	for (int i = 0; i < size; i++) {
		particles[i].update_position();
	}
}

// draw the snow object
// source: https://github.com/kodai100/OpenGL_MPMSnowSimulation2D
void SnowObject::draw() {
	//Snow particles
	if (SUPPORTS_POINT_SMOOTH) 
		glEnable(GL_POINT_SMOOTH);

	glPointSize(2);
	glBegin(GL_POINTS);
	for (int i = 0; i<size; i++) {
		Particle& p = particles[i];
		// We can use the particle's density to vary color
		float contrast = 0.6;
		float density = p.density / DENSITY * contrast;
		density += 1 - contrast;
		glColor3f(density, density, density);
		glVertex2f(p.position(0), p.position(1));
	}
	glEnd();
}
