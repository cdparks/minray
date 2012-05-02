#include <cstdio>
#include <cstdint>
#include <iostream>

using namespace std;

#include "Bitmap.h"

// BMP Structures from http://en.wikipedia.org/wiki/BMP_file_format

// BMP magic number
struct BMPMagic {
  unsigned char magic[2];
};
 
// BMP header
struct BMPHeader {
  uint32_t filesz;
  uint16_t creator1;
  uint16_t creator2;
  uint32_t bmp_offset;
};

// BMP info header
struct BMPInfo {
  uint32_t header_sz;
  int32_t width;
  int32_t height;
  uint16_t nplanes;
  uint16_t bitspp;
  uint32_t compress_type;
  uint32_t bmp_bytesz;
  int32_t hres;
  int32_t vres;
  uint32_t ncolors;
  uint32_t nimpcolors;
};

Bitmap::Bitmap(): width(0), height(0), pixels(NULL) {}

Bitmap::~Bitmap() {
	delete[] pixels;
	pixels = NULL;
}

Result Bitmap::load(const char *filename) {
	// Might be a reload;
	delete[] pixels;
	pixels = NULL;

	// Open file
	FILE *file = fopen(filename, "rb");
	if(file == NULL) {
		cerr << "Could not find file '" << filename << ".'" << endl;
		return FAILURE;
	}

	// Read magic number
	BMPMagic magic = {0};
	if(fread(&magic, 1, sizeof(magic), file) != sizeof(magic)) {
		cerr << "Could not read magic number from '" << filename << ".'" << endl;
		fclose(file);
		return FAILURE;
	}

	// Check magic number
	short bm = 'B' << 8 | 'M';
	short mb = 'M' << 8 | 'B';
	short filemagic;
	memcpy(&filemagic, &magic, 2);
	if(filemagic != bm && filemagic != mb) {
		cerr << "Wrong magic number - '" << filename << "' is not a BMP file." << endl;
		fclose(file);
		return FAILURE;
	}
	
	// Read header
	BMPHeader header = {0};
	if(fread(&header, 1, sizeof(header), file) != sizeof(header)) {
		cerr << "Could not read header from '" << filename << ".'" << endl;
		fclose(file);
		return FAILURE;
	}
	
	// Read info
	BMPInfo info = {0};
	if(fread(&info, 1, sizeof(info), file) != sizeof(info)) {
		cerr << "Could not read header info from '" << filename << ".'" << endl;
		fclose(file);
		return FAILURE;
	}

	// Check compression - we only read uncompressed RGB
	if(info.compress_type != BI_RGB) {
		cerr << "Could not read compressed BMP '" << filename << ".'" << endl;
		fclose(file);
		return FAILURE;
	}

	// Check bit depth - we only read 24bit color
	if(info.bitspp != 24) {
		cerr << "Could not read non-24-bit color BMP '" << filename << ".'" << endl;
		fclose(file);
		return FAILURE;
	}

	// Read pixels
	height = info.height;
	width = info.width;
	size = height * width * 3;
	pixels = new unsigned char[size];

	// Number of bytes in a row
	long rowsize = width * 3; 

	// Rows in the file are padded to end on a word boundary
	long paddedsize = rowsize;
	if(paddedsize % 4 != 0) {
		paddedsize = paddedsize + (4 - (paddedsize % 4));
	}

	// Row buffer
	unsigned char *row = new unsigned char[paddedsize];

	// Fill in pixels upside down - BMPS are stored with 0, 0 at the lower left corner
	for(long i = height - 1; i > -1; --i) {
		if(fread(row, 1, paddedsize, file) != paddedsize) {
			cerr << "Could not read pixel data from BMP '" << filename << ".'" << endl;
			fclose(file);
			delete[] row;
			delete[] pixels;
			return FAILURE;
		}
		// Copy row discarding padding
		memcpy(&pixels[i * rowsize], row, rowsize);
	}
	delete[] row;

	// Done reading
	fclose(file);

	// BGR -> RGB
	for(long i = 0; i < size; i += 3) {
		char x = pixels[i];
		pixels[i] = pixels[i + 2];
		pixels[i + 2] = x;
	}
	return SUCCESS;
}

long Bitmap::getHeight() const {
	return height;
}

long Bitmap::getWidth() const {
	return width;
}

long Bitmap::getSize() const {
	return size;
}

unsigned char *Bitmap::getPixels() {
	return pixels;
}
// Bilinear interpolation
glm::vec3 Bitmap::atUV(float u, float v) {
	float row = height - v * height;
	float col = u * width;

	long irow = clamp((long)row, 0L, height);
	long icol = clamp((long)col, 0L, width);
	long irowPlus = clamp(irow + 1L, 0L, height);
	long icolPlus = clamp(icol + 1L, 0L, width);

	float dr = row - irow;
	float dc = col - icol;

	glm::vec3 Q00 = glm::vec3(RGBV3(pixels, irow, icol)) / 255.0f;
	glm::vec3 Q01 = glm::vec3(RGBV3(pixels, irow, icolPlus)) / 255.0f;
	glm::vec3 Q10 = glm::vec3(RGBV3(pixels, irowPlus, icol)) / 255.0f;
	glm::vec3 Q11 = glm::vec3(RGBV3(pixels, irowPlus, icolPlus)) / 255.0f;

	glm::vec3 R1 = Q00 * (1.0f - dc) + Q01 * dc;
	glm::vec3 R2 = Q10 * (1.0f - dc) + Q11 * dc;
	return R1 * (1.0f - dr) + R2 * dr;
}
