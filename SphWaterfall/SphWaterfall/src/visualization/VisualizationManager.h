#include "Camera.h"
#include "util.h"
#include "../data/FluidParticle.h"

std::vector<Camera> cameras;

static void init(Vector3 cameraLocation, Vector3 cameraDirection, unsigned int frameWidth, unsigned int frameHeight) {
	Camera cam = Camera(cameraLocation, cameraDirection, frameWidth, frameHeight, cameras.size());
	cameras.emplace_back(cam);
}

static void addCamera(Vector3 cameraLocation, Vector3 cameraDirection, unsigned int frameWidth, unsigned int frameHeight) {
	Camera cam = Camera(cameraLocation, cameraDirection, frameWidth, frameHeight, cameras.size());
	cameras.emplace_back(cam);
}

static void debugRenderFrame(std::vector<FluidParticle> particles) {
	for (int i = 0; i < cameras.size(); i++) {
		cameras[i].debugRenderFrame(convertSphParticles(particles), 1);
	}
}