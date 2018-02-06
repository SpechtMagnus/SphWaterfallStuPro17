#pragma once
#include "SphNeighbourSearch.h"
#include "SimulationUtilities.h";

class SphNeighbourSearchFactory {
public:
	SphNeighbourSearchFactory();
	~SphNeighbourSearchFactory();

	static ISphNeighbourSearch* getInstance(int key);
private:
};