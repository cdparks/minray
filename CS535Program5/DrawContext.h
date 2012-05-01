#ifndef DRAW_CONTEXT_H
#define DRAW_CONTEXT_H

#include <vector>
#include <string>

using namespace std;

#include "Globals.h"
#include "Shader.h"
#include "Scene.h"

class DrawContext{
public:
	// Public callbacks for glut (cannot use instance methods for 
	// callbacks).
	static void Display(void);
	static void Keyboard(unsigned char key, int x, int y);
	static void Load(string filename);

private:
	// Get private singleton
	static DrawContext& get_instance();
	
	// Don't instantiate anywhere but from get_instance
	DrawContext();
	DrawContext(DrawContext const&);
	void operator=(DrawContext const&);

	// Private callbacks called by public callbacks.
	void on_display(void);
	void on_keyboard(unsigned char key, int x, int y);
	void on_load(string filename);

	// Drawing variables
	Scene scene;					// Ray tracer scene

	// State variables
	GLint width;					// Current width
	GLint height;					// Current height

};

#endif
