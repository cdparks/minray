#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

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
	glm::vec3 illuminate(Shape *shape, Material &m, glm::vec3 &viewer, glm::vec3 &intersection);
	glm::vec3 shade(Shape *shape, Material &m, glm::vec3 &viewer, glm::vec3 &intersection, glm::vec3 &light, glm::vec3 &intensity);
	void nudge(Shape *shape, Ray &ray);
	Ray Scene::reflect(Shape *shape, glm::vec3 &intersection, glm::vec3 &direction, glm::vec3 &normal);

	inline int Scene::startProgress() {
		start = clock();
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
		clock_t seconds = (clock() - start) / CLOCKS_PER_SEC;
		cout << "Elapsed time [mm:ss] - " << right << setw(2) << setfill('0') << seconds / 60;
		cout << ":" << setw(2) << setfill('0') << seconds % 60 << endl;
	}

	glm::vec3 ambient;
	vector<Shape*> shapes;
	vector<PointLight*> pointLights;
	vector<AreaLight*> areaLights;
	int height;
	int width;
	int maxlevel;
	bool antialiasing;
	clock_t start;
	float *pixels;
};

#endif