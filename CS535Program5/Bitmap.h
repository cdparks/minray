#ifndef BITMAP_H
#define BITMAP_H

#include "Globals.h"

class Bitmap {
public:
	// Construtor
	Bitmap();

	// Destructor
	~Bitmap();

	// Load 24bit bitmap from file
	Result load(const char *filename);

	// Getters
	unsigned char *getPixels();
	long getWidth() const;
	long getHeight() const;
	long getSize() const;
private:
	long width;
	long height;
	long size;
	unsigned char *pixels;
};

#endif
