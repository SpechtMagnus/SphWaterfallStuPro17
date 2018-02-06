#pragma once

#include "../data/Vector3.h"
#include "../data/FluidParticle.h"
#include <math.h>

const int bytesPerPixel = 3; /// red, green, blue
const int fileHeaderSize = 14;
const int infoHeaderSize = 40;


static Vector3 findRightSkalar(Vector3 vec, Vector3 right) {
	Vector3 left = Vector3(right.x * -1, right.y * -1, right.z * -1);

	if (vec.x >= 0) {
		if ((vec.z > 0 && right.x < 0) || (vec.z < 0 && right.x > 0) || (vec.z == 0 && right.z > 0)) return left;
	}
	else {
		if ((vec.z > 0 && right.x < 0) || (vec.z < 0 && right.x > 0) || (vec.z == 0 && right.z < 0)) return left;
	}

	return right;
}

static Vector3 findSkalarVectorWithYZero(Vector3 vector) {
	double x = 1;
	double y = 0;
	double z = vector.z == 0 ? 0.f : vector.x / (-1 * vector.z);

	return findRightSkalar(vector, Vector3(x, y, z));
}

static Vector3 findUpVector(Vector3 first, Vector3 second) {
	Vector3 up = Vector3(first.y * second.z - first.z * second.y,
		first.z * second.x - first.x * second.z,
		first.x * second.y - first.y * second.x);

	if (up.y > 0) return up;
	return Vector3(up.x * -1, up.y * -1, up.z * -1);
}

static double getLength(Vector3 vec) {
	return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

static Vector3 normalizeVector(Vector3 vec) {
	double len = getLength(vec);
	return Vector3(vec.x / len, vec.y / len, vec.z / len);
}

static std::vector<DebugObject> convertSphParticles(std::vector<FluidParticle> &particles) {
	std::vector<DebugObject> output;
	for (unsigned int i = 0; i < particles.size(); i++) {
		output.emplace_back(DebugObject(particles[i].position, 1));
	}

	return output;
}

static void writeFrameToBitmap(Frame f, const char* fileName, unsigned int w, unsigned int h) {
	FILE *file;
	unsigned char *img = NULL;
	int filesize = 54 + 3 * w*h;

	img = (unsigned char *)malloc(3 * w*h);
	memset(img, 0, 3 * w*h);

	for (int i = 0; i<w; i++)
	{
		for (int j = 0; j<h; j++)
		{
			int x = i; int y = (h - 1) - j;
			int r = f.getPixel(x, y).getRedValue();
			int g = f.getPixel(x, y).getGreenValue();
			int b = f.getPixel(x, y).getBlueValue();
			if (r > 255) r = 255;
			if (g > 255) g = 255;
			if (b > 255) b = 255;
			img[(x + y * w) * 3 + 2] = (unsigned char)(r);
			img[(x + y * w) * 3 + 1] = (unsigned char)(g);
			img[(x + y * w) * 3 + 0] = (unsigned char)(b);
		}
	}

	unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0 };
	unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0 };
	unsigned char bmppad[3] = { 0,0,0 };

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[4] = (unsigned char)(w);
	bmpinfoheader[5] = (unsigned char)(w >> 8);
	bmpinfoheader[6] = (unsigned char)(w >> 16);
	bmpinfoheader[7] = (unsigned char)(w >> 24);
	bmpinfoheader[8] = (unsigned char)(h);
	bmpinfoheader[9] = (unsigned char)(h >> 8);
	bmpinfoheader[10] = (unsigned char)(h >> 16);
	bmpinfoheader[11] = (unsigned char)(h >> 24);

	file = fopen(fileName, "wb");
	fwrite(bmpfileheader, 1, 14, file);
	fwrite(bmpinfoheader, 1, 40, file);
	for (int i = 0; i<h; i++)
	{
		fwrite(img + ((h-i-1)*w*bytesPerPixel), 3, w, file);
		fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, file);
	}

	free(img);
	fclose(file);
}