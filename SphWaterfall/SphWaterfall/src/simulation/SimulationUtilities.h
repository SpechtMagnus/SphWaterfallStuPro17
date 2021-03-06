#pragma once
#include "mpi.h"
#include "../data/Vector3.h"
#include "../data/SphParticle.h"

// for checking if a double is 0
#define EPSILON 1e-6

// kernel Influence radius
#define Q_MAX 1.2
// smoothing radius 
#define H 1.0
// domain dimesion
#define DOMAIN_DIMENSION Q_MAX * 2.0
// minimal density of a particle
#define FLUID_REFERENCE_DENSITY 1.0
// minimal density of a particle
#define STATIC_REFERENCE_DENSITY 1.0
// particle mass
#define FLUID_MASS 1.0
// particle mass
#define STATIC_MASS 2.0
// reference pressure
#define PRESSURE_CONSTANT 20.0

// size of the sources cubic spawn are
#define SOURCE_SIZE  Q_MAX * 2.0

// maximal velocity of a particle
#define MAX_VELOCITY 50
// default simulation time in seconds
#define DEFAULT_SIMULATION_TIME 100
// time in seconds one timestep takes
#define TIMESTEP_DURATION 0.03

// Sph Manager tags
#define META_RIM_TAG 0
#define EXCHANGE_TAG 1
#define EXPORT_TAG 2
#define EXPORT_PARTICLES_NUMBER_TAG 3
#define META_EXCHANGE_TAG 4
#define META_META_RIM_TAG 5
#define DENSITY_RIM_TAG 6
#define RIM_TAG 10

namespace SimulationUtilities {
	int hash(const Vector3&);
	Vector3 unhash(const int&);	
	int computeProcessID(const Vector3 position, const Vector3 domain_dimension);
	int computeProcessID(const int domain_id);
	int computeDomainID(const Vector3& position, const Vector3& domain_dimension);

	extern MPI_Comm slave_comm;
	extern int slave_comm_size;
}