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

	double poly_6_coefficient;
	double spikey_grad_coefficient;
	double viscosity_laplacian_coefficient;
	double viscosity_coefficient;

	std::unordered_map<int, ParticleDomain> domains;
	std::unordered_map<int, std::vector<SphParticle>> add_particles_map;
	std::unordered_map<SphParticle::ParticleType, std::unordered_map<int, std::vector<SphParticle*>>> process_map;
	std::unordered_map<SphParticle::ParticleType, std::unordered_map<int, std::vector<SphParticle>>> incoming;
	std::vector<std::vector<SphParticle*>> neighbour_particles;
	std::vector<Vector3> sources;

	ISphKernel* kernel;
	ISphNeighbourSearch* neighbour_search;
	SphKernelFactory kernel_factory;
	SphNeighbourSearchFactory neighbour_search_factory;

	void cleanUpAllParticles();
	void cleanUpFluidParticles();
	void cleanUpStaticParticles();

	std::vector<SphParticle> getNeighbours(int);
	void update();
	bool updateVelocity(SphParticle&, std::vector<SphParticle>&);
	Vector3 correctVelocity(SphParticle&, Vector3&, std::vector<SphParticle>&);
	Vector3 computeAcceleration(SphParticle&, std::vector<SphParticle>&);
	Vector3 computeDensityAcceleration(SphParticle&, std::vector<SphParticle>&);
	Vector3 computeViscosityAcceleration(SphParticle&, std::vector<SphParticle>&);
	void computeLocalDensity(SphParticle&, std::vector<SphParticle>&);
	double computeLocalPressure(SphParticle&);

	void exchangeParticles();
	void exchangeRimParticles(SphParticle::ParticleType);
	void exchangeRimDensity(SphParticle::ParticleType);
	void spawnSourceParticles();

	ParticleDomain& getParticleDomain(const int&);
	ParticleDomain& getParticleDomain(const Vector3&);
};