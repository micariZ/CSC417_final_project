#ifndef SHAPE_H
#define	SHAPE_H

#include <vector>
#include <math.h>
#include <GLFW/glfw3.h>
#include <Eigen/Dense>
#include <EigenTypes.h>

class Shape {
public:
	std::vector<Eigen::Vector2f> vertices;
	Eigen::Vector2f vel;
	
	Shape();
	Shape(Eigen::Vector2f vel);
	Shape(const Shape& orig);
	virtual ~Shape();
	
	//Add vertex to back of vertex list
	void addPoint(float x, float y);

	//Does this shape contain this point
	bool contains(float x, float y);

	//Compute area of shape
	float area();

	//Estimate volume, if this 2D object were actually 3D
	float volume();

	//Bounding box for shape
	void bounds(float bounds[4]);

	static Shape* generateSnowball(Eigen::Vector2f origin, float radius, Eigen::Vector2f vel);
	
	void draw();
};

#endif

