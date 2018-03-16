#pragma once
#include "Camera.h"
#include "util.h"
#include "../data/SphParticle.h"
#include "../data/ParticleIO.h"


static std::vector<Camera> cameras;
static Terrain terrainAA;

static void init(Vector3 cameraLocation, Vector3 cameraDirection, unsigned int frameWidth, unsigned int frameHeight) {
	Camera cam = Camera(cameraLocation, cameraDirection.normalize(), frameWidth, frameHeight, cameras.size());
	cam.renderGeometryFrame(terrainAA);
	cameras.emplace_back(cam);
}

static void addCamera(Vector3 cameraLocation, Vector3 cameraDirection, unsigned int frameWidth, unsigned int frameHeight) {
	Camera cam = Camera(cameraLocation, cameraDirection, frameWidth, frameHeight, cameras.size());
	cam.renderGeometryFrame(terrainAA);
	cameras.emplace_back(cam);
}

static void debugRenderFrame(std::vector<SphParticle> particles) {
	for (int i = 0; i < cameras.size(); i++) {
		cameras[i].debugRenderFrame(convertSphParticles(particles));
	}
}

static void importTerrain(Terrain t) {
	terrainAA = t;
}

static vector<SphParticle> generateDebugParticles(int count) {
	vector<SphParticle> particles;

	for (int i = 0; i < count; i++) {
		double x = double(rand() % 10000 - 5000) / 1000;
		double y = double(rand() % 10000 - 5000) / 1000;
		double z = double(rand() % 10000 - 5000) / 1000;

		SphParticle f = SphParticle(Vector3(x, y, z),Vector3(0,0,0), 1);
		particles.emplace_back(f);
	}
	return particles;

}

static void generateMultipleFrames(int frameCount, int particleCount) {
	unordered_map<int, vector<SphParticle>> frames;

	cout << "Generating " << frameCount << " frames...\n";

	for (int i = 0; i < frameCount; i++) {
		frames.insert_or_assign(i, generateDebugParticles(particleCount));
	}

	cout << "Frames generated\n";

	exportParticles(frames, "test.particles");
}

static void renderFrames(string fileName) {
	if (cameras.size() == 0) {
		cout << "Please add a camera first";
	}

	vector<vector<SphParticle>> frameParticles = importParticles(fileName);
	
	for (int g = 0; g < frameParticles.size(); g++) {
		cout << "Rendering Frame #" << g << "...\n";
		vector<ParticleObject> frame = convertFluidParticles(frameParticles.at(g));
		for (int i = 0; i < cameras.size(); i++) {
			cout << "\t...on Camera #" << i << "\n";
			Frame f = cameras.at(i).renderFrame(frame);

			writeFrameToBitmap(applySmoothingShader(f, 10), (("output/c" + std::to_string(i) + "f" + std::to_string(g)) + ".bmp").c_str(), f.getWidth(), f.getHeight());
		}
	}
}