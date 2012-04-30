#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"
#include "glm/ext.hpp"

struct Sphere : Shape {
	Sphere():
		Shape(),
		radius(0),
		position(0.0),
		diffuse(0.0),
		specular(0.0),
		shiny(0.0),
		refraction(0.0)
	{}

	virtual bool intersect(Ray &ray, glm::vec3 &intersection, glm::vec3 &normal) {
		return glm::intersectRaySphere(ray.origin, ray.direction, position, radius, intersection, normal);
	}

	float radius;
	glm::vec3 position;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shiny;
	float refraction;
};

#endif