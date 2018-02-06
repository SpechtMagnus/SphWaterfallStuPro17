#pragma once
#include <vector>

#include "../geometry/Terrain.h"
#include "../data/SphParticle.h"
#include "../data/StaticParticle.h"
#include "../data/Vector3.h"

class StaticParticleGenerator {
public:
	StaticParticleGenerator();
	std::vector<ISphParticle> generateStaticParticles(Terrain source);
	std::vector<ISphParticle> generateStaticParticles(Face face, float particleDistance);

	StaticParticleGenerator detectDuplicate(ISphParticle a, ISphParticle b);
	StaticParticleGenerator removeDuplicate(ISphParticle a, ISphParticle b);

};

