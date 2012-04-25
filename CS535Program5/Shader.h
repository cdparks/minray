#ifndef SHADER_H
#define SHADER_H

#include "Globals.h"

class Shader {
public:
	// Constructor
	Shader();

	// Load, compile, and link vertex and fragment shader
	Result load(const char *vertfile, const char *fragfile);

	// Get Opengl object id
	GLuint ID() const;
private:
	// Compile individual shader
	GLuint compile(const char *filename, GLenum type);

	// Read file in
	unsigned char *readshader(const char *filename);
	GLuint id;
};

#endif