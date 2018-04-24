#pragma once

#include "Camera.h"
#include "util.h"
#include "../data/SphParticle.h"
#include "../data/ParticleIO.h"
#include <string>


class VisualizationManager {
public:
	static void init(Vector3 cameraLocation, unsigned int frameWidth, unsigned int frameHeight, int rank);

	static void importTerrain(Terrain t, bool isGate);

	static void setSwitchFrame(int switch_frame);

	static void debugRenderFrame(std::vector<SphParticle> particles, string fileName);

	static void renderFrames(string inputFileName);

	static void renderFramesDistributed(string inputFileName, int rank);

	static void generateFrames(int frameCount, int particleCount);

	static void debug();

private:
	static vector<SphParticle> generateDebugParticles(int count);

	static bool initilaized;

	static Camera camera;
	static Terrain terrain;
	static Terrain gate;
	static int switch_frame;
};
