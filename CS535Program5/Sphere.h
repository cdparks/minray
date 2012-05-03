#ifndef SPHERE_H
#define SPHERE_H
// Simple sphere object for raytracing

#include "Shape.h"
#include "glm/ext.hpp"

struct Sphere : Shape {
	// Constructor
	Sphere():
		Shape(),
		radius(0),
		position(0.0),
		diffuse(0.0),
		specular(0.0),
		shiny(0.0),
		reflection(0.0)
	{}

	// If ray itersects Sphere, update t and return true
	virtual bool intersect(Ray &ray, float &t) {
		// b and c from quadratic equation (a is factored out)
		float b = (glm::dot(ray.direction, ray.origin) - glm::dot(ray.direction, position)) * 2;
		float c = glm::dot(ray.origin, ray.origin) + glm::dot(position, position) - glm::dot(ray.origin, position) * 2 - radius * radius;

		// Determinant
		float d = b * b - 4 * c;
		if(d < 0) {
			return false;
		}

		// Compute smaller t first
		float sqrtD = sqrt(d);
		t = (-b - sqrtD) / 2;
		if(t <= EPS) {
			// Try larger t
			t = (-b + sqrtD) / 2;
			if(t <= EPS) {
				return false;
			}
		}
		return true;
	}

	// Most attributes are the same at all points on sphere. Normal
	// needs to be calculated though.
	virtual void shading(glm::vec3 &intersection, Material &m) {
		m.normal = glm::normalize((intersection - position) / radius);
		m.reflection = reflection;
		m.diffuse = diffuse;
		m.specular = specular;
		m.shiny = shiny;
	}

	float radius;
	glm::vec3 position;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shiny;
	float reflection;
};

#endif