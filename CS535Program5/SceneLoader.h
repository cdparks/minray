#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H
// Read file and add objects to scene

#include <fstream>
#include <string>
#include <sstream>
using namespace std;

#include "Globals.h"
#include "Scene.h"

class SceneLoader {
public:
	// Constructor
	SceneLoader():lineno(0), fin() {}

	// Load objects from file to scene
	Result load(string filename, Scene &scene);

private:
	// Advance file to next non-comment, non-empty line.
	// Return false at EOF
	bool advance();

	// Return next line in stringstream
	stringstream line();

	// Read objects into scenes
	Result readSphere(Scene &scene);
	Result readTriangle(Scene &scene);
	Result readAreaLight(Scene &scene);
	Result readPointLight(Scene &scene);

	// Print error, close file, and exit
	Result loadError(string message);

	// Compare expected tag to beginning of current line and
	// update stringstream
	Result readTag(string expected, stringstream &ss);

	int lineno;			// Current line number
	ifstream fin;		// file stream
	string buffer;		// line buffer
};

#endif