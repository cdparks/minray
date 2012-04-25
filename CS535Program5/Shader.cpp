#include <cstdio>
#include <iostream>
using namespace std;

#include "Shader.h"

Shader::Shader(): id(0) {}

GLuint Shader::ID() const { return id; };

// Load, compile, link
Result Shader::load(const char *vertfile, const char *fragfile) {
	GLint linked, bytes;
	char errmsg[512];

	// Compile shaders
	GLuint vertshader = compile(vertfile, GL_VERTEX_SHADER_ARB);
	GLuint fragshader = compile(fragfile, GL_FRAGMENT_SHADER_ARB);
	if(vertshader == 0 || fragshader == 0) {
		return FAILURE;
	}

	id = glCreateProgram();

	// Attach to program
    glAttachShader(id, vertshader);
    glAttachShader(id, fragshader);

	// Link program
    glLinkProgram(id);
    glGetProgramiv(id, GL_OBJECT_LINK_STATUS_ARB, &linked);
    if(!linked){
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &bytes);
		glGetProgramInfoLog(id, sizeof(errmsg), &bytes, errmsg);
		errmsg[bytes] = '\0';
		cerr << "Shader Link Error for '" << vertfile << "' and '" << fragfile << "': " << errmsg << endl;
		id = 0;
		return FAILURE;
	}
	return SUCCESS;
}

// Compile vertex or fragment shader
GLuint Shader::compile(const char *filename, GLenum type) {
	unsigned char *source = NULL;

	GLint compiled, bytes;
	char errmsg[512];

	// Read source
	source = readshader(filename);
	if(source == NULL) {
		return 0;
	}
	
	// Create shader and compile
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, (const char**)&source, NULL);
	glCompileShader(shader);
    delete[] source;

	// Check status
	glGetObjectParameteriv(shader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
	if(!compiled) {
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &bytes);
		glGetShaderInfoLog(shader, sizeof(errmsg), &bytes, errmsg);
		errmsg[bytes] = '\0';
		cerr << "Shader Compile Error for '" << filename << "': " << errmsg << endl;
		return 0;
	}
	
	return shader;
}

// Make you a big bag o' bytes
unsigned char *Shader::readshader(const char *filename) {

	// Open file
    FILE *file = fopen(filename, "rb");
    if( file == NULL ) {
        cerr << "Cannot open shader file '" << filename << ".'" << endl;
        return NULL;
    }

	// Get size
	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);
	rewind(file);

	// Read source into buffer
    unsigned char *buffer = new unsigned char[size + 1];
	size_t bytes;
	if((bytes = fread(buffer, 1, size, file)) != size) {
		cerr << "Cannot read shader file '" << filename << ".'" << endl;
		delete[] buffer;
		fclose(file);
        return NULL;
	}

    buffer[bytes] = 0;
	fclose(file);
	return buffer;
}
