#ifndef SHAPE_H
#define SHAPE_H
// Base class of Sphere and Triangle

#include "Globals.h"
#include "Ray.h"

// Shared interface for shading
struct Material {
	glm::vec3 normal;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shiny;
	float reflection;
};

struct Shape {
	// Constructor
	Shape() {}

	// Subclass must implement intersect - return true on intersect
	// and set t.
	virtual bool intersect(Ray &ray, float &t) = 0;

	// Subclass must implement shading - fill in material with properties
	// at intersection point
	virtual void shading(glm::vec3 &intersection, Material &m) = 0;
};

#endif