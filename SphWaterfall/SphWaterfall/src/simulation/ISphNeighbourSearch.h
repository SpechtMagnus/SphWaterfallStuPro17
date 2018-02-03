#pragma once
#include "../data/SphParticle.h"
#include "ParticleDomain.h"
#include <unordered_map>

class ISphNeighbourSearch {
public:
	virtual std::vector<SphParticle> findNeigbours(SphParticle, std::unordered_map<int, ParticleDomain> domains) = 0;
	virtual std::vector<ParticleDomain> findRelevantNeighbourDomains(SphParticle, std::unordered_map<int, ParticleDomain> domains) = 0;
};