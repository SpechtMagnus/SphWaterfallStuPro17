#pragma once
#include "mpi.h"
#include "SphKernelFactory.h"
#include "SphNeighbourSearchFactory.h"
#include "SimulationUtilities.h"

#include <vector>
#include <array>
#include <unordered_map>
#include <iterator>
#include <random>
#include <functional>

class SphManager {
public:
	SphManager(const Vector3&, int number_of_timesteps, double timestep_duration);
	~SphManager();

	void simulate();
	void exportParticles();
	void add_particles(const std::vector<SphParticle>&);
	void setSink(const double&);
	void addSource(const Vector3&);
	const Vector3& getDomainDimensions() const;

private:
	int mpi_rank;
	Vector3 domain_dimensions;
	int number_of_timesteps;
	double sink_height;
	double const timestep_duration;
	double half_timestep_duration;
	Vector3 const gravity_acceleration;

	std::unordered_map<int, ParticleDomain> domains;
	std::unordered_map<int, std::vector<SphParticle>> add_particles_map;
	std::vector<std::pair<SphParticle, std::vector<SphParticle>>> neighbour_fluid_particles;
	std::vector<std::pair<SphParticle, std::vector<SphParticle>>> neighbour_static_particles;
	std::vector<Vector3> sources;

	ISphKernel* kernel;
	ISphNeighbourSearch* neighbour_search;
	SphKernelFactory kernel_factory;
	SphNeighbourSearchFactory neighbour_search_factory;

	void cleanUpAllParticles();
	void cleanUpFluidParticles();
	void cleanUpStaticParticles();

	void update();
	bool updateVelocity(SphParticle&);
	void interpolateWithNeighbourVelocities(Vector3&, const std::vector<SphParticle>&);
	Vector3 computeAcceleration(SphParticle&, std::vector<SphParticle>&, std::vector<SphParticle>&);
	Vector3 computeDensityAcceleration(SphParticle&, std::vector<SphParticle>&, std::vector<SphParticle>&);
	Vector3 computeViscosityAcceleration(SphParticle&, std::vector<SphParticle>&, std::vector<SphParticle>&);
	void computeLocalDensity(SphParticle&);
	double computeLocalPressure(SphParticle&);

	void exchangeParticles();
	void exchangeRimParticles(SphParticle::ParticleType);
	void spawnSourceParticles();

	ParticleDomain& getParticleDomain(const int&);
	ParticleDomain& getParticleDomain(const Vector3&);
};