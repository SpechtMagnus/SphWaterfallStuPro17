#pragma once
#include "vector"

#include "../data/Vector3.h"
#include "../data/SphParticle.h" 

class FluidParticleGenerator {
public:
	FluidParticleGenerator();
	FluidParticleGenerator(double disposalHeight, Vector3 spawn);
	SphParticle spawnParticle();
	void collectInactive();



private:
	std::vector<SphParticle> activeParticles;
	std::vector<SphParticle> inactiveParticles;
	double disposalHeight;
	Vector3 spawn;
};