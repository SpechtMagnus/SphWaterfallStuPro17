#pragma once
#include "mpi.h"
#include "SphKernelFactory.h"
#include "SphNeighbourSearchFactory.h"
#include "SimulationUtilities.h";

#include <vector>
#include <array>
#include <unordered_map>
#include <iterator>

class SphManager {
public:
	SphManager(const Vector3&, double simulation_time, double timestep_duration);
	~SphManager();

	void simulate();
	void add_particles(const std::vector<SphParticle>&);
private:
	Vector3 domain_dimensions;
	double simulation_time;
	double timestep_duration;
	std::unordered_map<int, ParticleDomain> domains;
	std::unordered_map<int, std::pair<SphParticle, std::vector<SphParticle>>> neighbour_particles;
	ISphKernel* kernel;
	ISphNeighbourSearch* neighbour_search;
	SphKernelFactory kernel_factory;
	SphNeighbourSearchFactory neighbour_search_factory;

	void update(double timestep);
	void updateVelocity(SphParticle& particle, double timestep);
	Vector3 computeAcceleration(SphParticle& particle);
	Vector3 computeDensityAcceleration(SphParticle& particle);
	Vector3 computeViscosityAcceleration(SphParticle& particle);
	void computeLocalDensity(SphParticle&);
	double computeLocalPressure(SphParticle&);
	void findNeighbourDomains(ParticleDomain);

	MPI_Request requestRimParticles(const Vector3&, const Vector3&);
	void exchangeParticles();
	void exchangeRimParticles();	ParticleDomain& getParticleDomain(const int&);
	void sendRimParticles(const int&, const int&);
};