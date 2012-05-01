#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shape.h"
#include "Bitmap.h"
#include "glm/ext.hpp"

struct Triangle : Shape {
	Triangle():
		Shape(),
		texture(NULL)
	{}

	// Extremely helpful:
	// http://www.cs.washington.edu/education/courses/csep557/10au/lectures/triangle_intersection.pdf
	virtual bool intersect(Ray &ray, float &t) {
		// Get normal of plane
		glm::vec3 normal = glm::normalize(glm::cross(vertices[1] - vertices[0], vertices[2] - vertices[0]));

		// d is dot(normal, any point in plane)
		float d = glm::dot(normal, vertices[0]);

		// Compare direction of ray direction and normal
		float denom = glm::dot(normal, ray.direction);

		// Ray direction and normal are parallel
		if(fabs(denom) < EPS) {
			return false;
		}

		// Solve for t
		t = (d - glm::dot(normal, ray.origin)) / denom;

		// Get hit point
		glm::vec3 hit = ray.origin + ray.direction * t;

		// Check hit is inside triangle
		if(glm::dot(normal, glm::cross(vertices[1] - vertices[0], hit - vertices[0])) < 0) {
			return false;
		}
		if(glm::dot(normal, glm::cross(vertices[2] - vertices[1], hit - vertices[1])) < 0) {
			return false;
		}
		if(glm::dot(normal, glm::cross(vertices[0] - vertices[2], hit - vertices[2])) < 0) {
			return false;
		}
		return true;
	}

	glm::vec3 transform(glm::vec3 &barycentric, glm::vec3 values[3]) {
		return barycentric.x * values[0] + barycentric.y * values[1] + barycentric.z * values[2];
	}

	float transform(glm::vec3 &barycentric, float values[3]) {
		return barycentric.x * values[0] + barycentric.y * values[1] + barycentric.z * values[2];
	}

	float transform(glm::vec3 &barycentric, float x, float y, float z) {
		return barycentric.x * x + barycentric.y * y + barycentric.z * z;
	}

	// I was lost, so I looked here:
	// http://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
	virtual void shading(glm::vec3 &intersection, Phong &p) {
		glm::vec3 v0 = vertices[1] - vertices[0];
		glm::vec3 v1 = vertices[2] - vertices[0];
		glm::vec3 v2 = intersection - vertices[0];

		float d00 = glm::dot(v0, v0);
		float d01 = glm::dot(v0, v1);
		float d11 = glm::dot(v1, v1);
		float d20 = glm::dot(v2, v0);
		float d21 = glm::dot(v2, v1);
		float denom = d00 * d11 - d01 * d01;

		float v = (d11 * d20 - d01 * d21) / denom;
		float w = (d00 * d21 - d01 * d20) / denom;
		float u = 1.0f - v - w;

		glm::vec3 barycentric(u, v, w);

		p.normal = glm::normalize(transform(barycentric, normals));
		glm::vec3 tex(1);
		if(texture) {
			float u = transform(barycentric, texcoords[0].s, texcoords[1].s, texcoords[2].s);
			float v = transform(barycentric, texcoords[0].t, texcoords[1].t, texcoords[2].t);
			tex = texture->atUV(u, v);
		}
		p.diffuse = tex * transform(barycentric, diffuse);
		p.specular = transform(barycentric, specular);
		p.shiny = transform(barycentric, shiny);
	}

	glm::vec3 vertices[3];
	glm::vec3 normals[3];
	glm::vec2 texcoords[3];
	glm::vec3 diffuse[3];
	glm::vec3 specular[3];
	float shiny[3];
	float refraction[3];
	Bitmap *texture;
};

#endif