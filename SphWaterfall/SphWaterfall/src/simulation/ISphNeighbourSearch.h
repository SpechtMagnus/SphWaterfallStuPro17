#pragma once
#include "../data/SphParticle.h"
#include "ParticleDomain.h"
#include "SimulationUtilities.h";

#include <unordered_map>
#include <set>

using namespace SimulationUtilities;

class ISphNeighbourSearch {
public:
	virtual std::vector<SphParticle> findNeigbours(SphParticle particle, std::vector<SphParticle> neighbour_particles) = 0;
	virtual std::set<int> findRelevantNeighbourDomains(SphParticle particle, Vector3 dimension) = 0;

private:
};