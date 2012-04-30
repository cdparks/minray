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
	static void Mouse(int button, int state, int x, int y);
	static void Motion(int x, int y);
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
	void on_mouse(int button, int state, int x, int y);	
	void on_motion(int x, int y);
	void on_load(string filename);

	// Write text to screen
	void write(ostringstream &os, GLfloat x, GLfloat y, float *color);

	// Draw menu and mouse position
	void draw_interface(void);

	// Drawing variables
	Shader program;					// Vertex and Fragment shader
	Scene scene;					// Ray tracer scene

	// State variables
	bool draw_menu;					// Should we draw the help menu?
	bool left_down;					// Is the left mouse button down?
	bool right_down;				// Is the right mouse button down?
	Point2D mouse;					// Last known cursor position
	float pitch;					// Rotation around X
	float yaw;						// Rotation around Y
	GLint width;					// Current width
	GLint height;					// Current height

	static const float RESET_YAW;
	static const float RESET_PITCH;
};

#endif
