#pragma once
#include "SphNeighbourSearch.h"

SphNeighbourSearch::SphNeighbourSearch(){

}

SphNeighbourSearch::~SphNeighbourSearch() {

}

std::vector<SphParticle> SphNeighbourSearch::findNeigbours(SphParticle particle, std::vector<SphParticle> neighbour_particles) {
	std::vector<SphParticle> neighbours = std::vector<SphParticle>();

	//std::cout << "find neigbours" << std::endl;

	for (auto each_particle : neighbour_particles) {
		if (isInInfluentialRadius(particle.position, each_particle.position)) {
			neighbours.push_back(each_particle);
		}
	}
	
	return neighbours;
}

std::set<int> SphNeighbourSearch::findRelevantNeighbourDomains(SphParticle particle, Vector3 dimension) {
	std::set<int> neighbour_domain_ids = std::set<int>();

	std::cout << "find relevant neighbours" << std::endl;

	// center point
	Vector3 testing_point = particle.position;
	neighbour_domain_ids.insert(hash(testing_point));

	// front point
	testing_point = particle.position + (Vector3(0, 0, 1) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	// back point
	testing_point = particle.position + (Vector3(0, 0, -1) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	// left point
	testing_point = particle.position + (Vector3(-1, 0, 0) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	// right point
	testing_point = particle.position + (Vector3(1, 0, 0) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	// top point
	testing_point = particle.position + (Vector3(0, 1, 0) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	// bottom point
	testing_point = particle.position + (Vector3(0, -1, 0) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));

	// front bottom point
	testing_point = particle.position + (Vector3(0, 1, 1) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	// front top point
	testing_point = particle.position + (Vector3(0, -1, 1) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	// front left point
	testing_point = particle.position + (Vector3(-1, 0, 1) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	// front right point
	testing_point = particle.position + (Vector3(1, 0, 1) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	// front left bottom point
	testing_point = particle.position + (Vector3(-1, -1, 1) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	// front right bottom point
	testing_point = particle.position + (Vector3(1, -1, 1) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	// front left top point
	testing_point = particle.position + (Vector3(-1, 1, 1) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	// front right top point
	testing_point = particle.position + (Vector3(1, 1, 1) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));

	// back top point
	testing_point = particle.position + (Vector3(0, 1, -1) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	// back bottom point
	testing_point = particle.position + (Vector3(0, -1, -1) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	// back left point
	testing_point = particle.position + (Vector3(-1, 0, -1) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	// back right point
	testing_point = particle.position + (Vector3(1, 0, -1) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	// back left bottom point
	testing_point = particle.position + (Vector3(-1, -1, -1) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	// back right bottom point
	testing_point = particle.position + (Vector3(1, -1, -1) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	// back left top point
	testing_point = particle.position + (Vector3(-1, 1, -1) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	// back right top point
	testing_point = particle.position + (Vector3(1, 1, -1) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));

	// left top point
	testing_point = particle.position + (Vector3(-1, 1, 0) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	// left bottom point
	testing_point = particle.position + (Vector3(-1, -1, 0) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	// right top point
	testing_point = particle.position + (Vector3(1, 1, 0) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	// right bottom point
	testing_point = particle.position + (Vector3(1, -1, 0) * Q_MAX);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
	
	//for (auto neighbour : neighbour_domain_ids) {
	//	std::cout << "neighbour domain id:" << neighbour << std::endl;
	//}

	return neighbour_domain_ids;
}

bool SphNeighbourSearch::isInInfluentialRadius(Vector3 particle, Vector3 potential_neighbour) {
	return (((potential_neighbour.x - particle.x) * (potential_neighbour.x - particle.x)) + 
		((potential_neighbour.y - particle.y) * (potential_neighbour.y - particle.y)) + 
		((potential_neighbour.z - particle.z) * (potential_neighbour.z - particle.z))) <  (Q_MAX * Q_MAX);
}
