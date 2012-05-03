#ifndef AREA_LIGHT_H
#define AREA_LIGHT_H
// Implements an area light for drawing soft shadows

#include "Globals.h"
#include "glm/glm.hpp"

struct AreaLight {
	glm::vec3 position[4];
	glm::vec3 color;
	glm::vec3 samples[AREASAMPLE * AREASAMPLE];

	// Precompute simulated point lights as samples of area light
	void precompute() {
		float toUV = 1.0f / AREASAMPLE;
		for(int i = 0; i < AREASAMPLE; ++i) {
			float v = toUV * i;
			for(int j = 0; j < AREASAMPLE; ++j) {
				float u = toUV * j;
				glm::vec3 R1 = glm::mix(position[0], position[3], u);
				glm::vec3 R2 = glm::mix(position[1], position[2], u);
				samples[i * AREASAMPLE + j] = glm::mix(R1, R2, v);
			}
		}
	}

	// Get the simulated point light at row, col
	glm::vec3 sample(int row, int col) {
		return samples[row * AREASAMPLE + col];
	}
};

#endif