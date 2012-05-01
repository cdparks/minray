#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

#include "Scene.h"
#include "SceneLoader.h"

Scene::Scene(int height, int width):
	height(height),
	width(width),
	pixels(NULL)
{	
	pixels = new float[height * width * 3];
}

Result Scene::load(string filename) {
	destroy();
	return SceneLoader().load(filename, *this);
}

void Scene::status() {
	cout << "Shapes:       " << shapes.size() << endl;
	cout << "Point Lights: " << pointLights.size() << endl;
	cout << "Area Lights:  " << areaLights.size() << endl;
}

Scene::~Scene() {
	delete pixels;
	pixels = NULL;
	destroy();
}

void Scene::destroy() {
	for(size_t i = 0; i < shapes.size(); ++i) {
		delete shapes[i];
	}
	shapes.clear();

	for(size_t i = 0; i < pointLights.size(); ++i) {
		delete pointLights[i];
	}
	pointLights.clear();

	for(size_t i = 0; i < areaLights.size(); ++i) {
		delete areaLights[i];
	}
	areaLights.clear();
}

void Scene::add(Shape *shape) {
	shapes.push_back(shape);
}

void Scene::add(PointLight *light) {
	pointLights.push_back(light);
}

void Scene::add(AreaLight *light) {
	areaLights.push_back(light);
}

void Scene::setAmbient(glm::vec3 &ambient) {
	this->ambient = ambient;
}

void Scene::raytrace(int antialiasing) {
	glm::vec3 eye(0.0, 0.0, 1.5);
	float sample = pow(2.0, antialiasing);
	float inc = 1.0f / sample;
	float adj = inc * inc;
	int step = startProgress(antialiasing, height);
	for(int i = 0; i < height; ++i) {
		for(int j = 0; j < width; ++j) {
			glm::vec3 color(0);
			for(float isample = 0.0f; isample < 1.0f; isample += inc) {
				float y = (i + isample) * (1.0 / height) * 2 - 1;
				for(float jsample = 0.0f; jsample < 1.0f; jsample += inc) {
					float x = (j + jsample) * (1.0 / width) * 2 - 1;
					Ray ray(eye, glm::normalize(glm::vec3(x, y, 0.0) - eye)); 
					color += trace(ray, 0);
				}
			}
			color *= adj;
			pixels[AT_R(i, j)] = color.r;
			pixels[AT_G(i, j)] = color.g;
			pixels[AT_B(i, j)] = color.b;
		}
		updateProgress(i, step);
	}
	endProgress();
}

void Scene::draw() {
	glRasterPos2i(0, 0);
	glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);
}

glm::vec3 Scene::trace(Ray &ray, int level) {
	if(level >= 10) {
		return glm::vec3(0);
	}

	Shape *shape = NULL;
	float t, closest = 10000000.0f;
	for(size_t i = 0; i < shapes.size(); ++i) {
		if(shapes[i]->intersect(ray, t)) {
			if(t < closest) {
				closest = t;
				shape = shapes[i];
			}
		}
	}

	if(shape == NULL) {
		return glm::vec3(0);
	} else {
		glm::vec3 intersection = shape->intersection(ray, closest);
		glm::vec3 normal = shape->normal(intersection);
		glm::vec3 color = ambient * shape->color(intersection);
		for(size_t i = 0; i < pointLights.size(); ++i) {
			glm::vec3 lightDir = glm::normalize(pointLights[i]->position - intersection);
			color += pointLights[i]->color * shape->phong(intersection, -ray.direction, normal, lightDir);
		}
		return glm::clamp(color, 0.0f, 1.0f);
	}
}
