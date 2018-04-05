#pragma once
#include "mpi.h"
#include "SphKernelFactory.h"
#include "SphNeighbourSearchFactory.h"
#include "SimulationUtilities.h"

#include <vector>
#include <array>
#include <unordered_map>
#include <iterator>

class SphManager {
public:
	SphManager();
	SphManager(const Vector3&);
	~SphManager();

	void simulate(int number_of_timesteps);
	void add_particles(const std::vector<SphParticle>&);
	void exportParticles();

private:
	int mpi_rank;
	Vector3 domain_dimensions;
	double sink_height;
	double half_timestep_duration;
	Vector3 const gravity_acceleration;
	std::unordered_map<int, ParticleDomain> domains;
	std::unordered_map<int, std::vector<SphParticle>> add_particles_map;
	std::vector<std::pair<SphParticle, std::vector<SphParticle>>> neighbour_particles;
	ISphKernel* kernel;
	ISphNeighbourSearch* neighbour_search;
	SphKernelFactory kernel_factory;
	SphNeighbourSearchFactory neighbour_search_factory;

	ParticleDomain& getParticleDomain(const int&);
	ParticleDomain& getParticleDomain(const Vector3&);
	
	void cleanUpAllParticles();
	void cleanUpFluidParticles();
	void cleanUpStaticParticles();

	void update();
	bool updateVelocity(SphParticle& particle);
	Vector3 computeAcceleration(SphParticle& particle);
	Vector3 computeDensityAcceleration(SphParticle& particle);
	Vector3 computeViscosityAcceleration(SphParticle& particle);
	void computeLocalDensity(SphParticle&);
	double computeLocalPressure(SphParticle&);
	void exchangeParticles();
	void exchangeRimParticles(SphParticle::ParticleType);
	void setSink(double);
};