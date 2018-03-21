#pragma once
#include "ParticleDomain.h"

class DomainDecomposer {
public:
	DomainDecomposer();
	~DomainDecomposer();
	
	bool decideDecomposition(ParticleDomain domains[]);
	void decompose(ParticleDomain domains[]);
private:
	std::unordered_map<int, ParticleDomain> domains;
};