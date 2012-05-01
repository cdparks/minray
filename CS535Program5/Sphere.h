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

	virtual bool intersect(Ray &ray, float &t) {
		float b = (glm::dot(ray.direction, ray.origin) - glm::dot(ray.direction, position)) * 2;
		float c = glm::dot(ray.origin, ray.origin) + glm::dot(position, position) - glm::dot(ray.origin, position) * 2 - radius * radius;
		float d = b * b - 4 * c;
		if(d < 0) {
			return false;
		}
		float sqrtD = sqrt(d);
		float t0 = (-b - sqrtD) / 2;
		if(t0 <= 0) {
			float t1 = (-b + sqrtD) / 2;
			if(t1 <= 0) {
				return false;
			} else {
				t = t1;
			}
		} else {
			t = t0;
		}
		return true;
	}

	virtual glm::vec3 intersection(Ray &ray, float t) {
		return ray.origin + ray.direction * t;
	}

	virtual glm::vec3 normal(glm::vec3 &intersection) {
		return glm::normalize((intersection - position) / radius);
	}

	virtual glm::vec3 color(glm::vec3 &intersection) {
		return diffuse;
	}

	virtual glm::vec3 phong(glm::vec3 &intersection, glm::vec3 &viewer, glm::vec3 &normal, glm::vec3 &light) {				
			glm::vec3 diff = diffuse * max(0.0f, glm::dot(normal, light));
			glm::vec3 spec = specular * pow(max(0.0f, glm::dot(viewer, glm::reflect(-light, normal))), shiny);
			return diff + spec;
	}

	float radius;
	glm::vec3 position;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shiny;
	float refraction;
};

#endif