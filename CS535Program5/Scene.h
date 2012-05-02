#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include <fstream>

using namespace std;

#include "Globals.h"
#include "Ray.h"
#include "Sphere.h"
#include "Triangle.h"
#include "AreaLight.h"
#include "PointLight.h"
#include "glm/glm.hpp"

#define AT(row, col) ((row) * width + (col))

class Scene {
public:
	Scene();
	Scene(int width, int height, int maxlevel, bool antialiasing);
	~Scene();
	Result load(string filename);
	void status();
	void add(Shape *shape);
	void add(PointLight *light);
	void add(AreaLight *light);
	void setAmbient(glm::vec3 &light);
	void raytrace();
	void draw();
private:
	void destroy();
	glm::vec3 trace(Ray &ray, int level);
	void nudge(Shape *shape, Ray &ray);
	Ray Scene::reflect(Shape *shape, glm::vec3 &intersection, glm::vec3 &direction, glm::vec3 &normal);

	inline int Scene::startProgress() {
		cout << "Ray tracing progress" << endl;
		for(int i = 0; i < 20; ++i) {
			cout << "=";
		}
		cout << endl;
		return height / 20;
	}

	inline void Scene::updateProgress(int i, int step) {
		if(i && i % step == 0) {
			cout << "=";
		}
	}

	inline void Scene::endProgress() {
		cout << "=" << endl;
	}

	glm::vec3 ambient;
	vector<Shape*> shapes;
	vector<PointLight*> pointLights;
	vector<AreaLight*> areaLights;
	int height;
	int width;
	int maxlevel;
	bool antialiasing;
	float *pixels;
};

#endif