#pragma once
#include <vector>

#include "../geometry/Terrain.h"
#include "../data/SphParticle.h"
#include "../data/StaticParticle.h"
#include "../data/Vector3.h"

class StaticParticleGenerator {
public:
	static std::vector<SphParticle> generateStaticParticles(Terrain source);
	static std::vector<SphParticle> generateStaticParticles(Face face, float particleDistance);

	static StaticParticleGenerator detectDuplicate(SphParticle a, SphParticle b);
	static StaticParticleGenerator removeDuplicate(SphParticle a, SphParticle b);
};

