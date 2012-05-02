#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include <fstream>
#include <string>
#include <sstream>
using namespace std;

#include "Globals.h"
#include "Scene.h"

class SceneLoader {
public:
	SceneLoader():lineno(0), fin() {}
	Result load(string filename, Scene &scene);
private:
	bool advance();
	stringstream line();
	Result readSphere(Scene &scene);
	Result readTriangle(Scene &scene);
	Result readAreaLight(Scene &scene);
	Result readPointLight(Scene &scene);
	Result loadError(string message);
	Result readTag(string expected, stringstream &ss);
	int lineno;
	ifstream fin;
	string buffer;
};

#endif