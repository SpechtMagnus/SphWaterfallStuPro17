#pragma once
#include "Frame.h"
#include "DebugObject.h"
#include "../data/SphParticle.h"
#include "../data/Vector3.h"
#include <vector>
#include <string>

class Camera {
public:
	Camera(Vector3 location, Vector3 direction, unsigned int width, unsigned int height, unsigned int ID);

	void debugRenderFrame(std::vector<DebugObject> particles, int frameID);

	void renderFrame(std::vector<SphParticle> particles); //Vector is only a placeholder here as the data structure isnt decided yet

	void mergeFramesAndFlushVideo(std::string file);
	void outputDebugFrame(Frame f, const char* fileName);

	Vector3 getLocation();
	Vector3 getDirection();

private:

	Pixel castDebugRay(Ray ray, std::vector<DebugObject> particles);

	Vector3 location;
	Vector3 direction;
	unsigned int width;
	unsigned int height;
	unsigned int ID;
	std::vector<Frame> frames;

	Frame getFrame(unsigned int frameID);
};