#pragma once
#include "mpi.h"
#include "../data/Vector3.h"
#include "../data/SphParticle.h"

// kernel Influence radius
#define Q_MAX 1.2
// smoothing radius 
#define H 1.0
// max Influence radius
#define R_MAX Q_MAX * H
// domain dimesion
#define DOMAIN_DIMENSION R_MAX * 2.0
// minimal density of a particle
#define FLUID_REFERENCE_DENSITY 1.0
// minimal density of a particle
#define STATIC_REFERENCE_DENSITY 1.0
// particle mass
#define FLUID_MASS 1.0
// particle mass
#define STATIC_MASS 5.0

// default simulation time in seconds
#define DEFAULT_SIMULATION_TIME 3
// time in seconds one timestep takes
#define TIMESTEP_DURATION 0.03

// Sph Manager tags
#define META_TAG 0
#define EXCHANGE_TAG 1
#define EXPORT_TAG 2
#define EXPORT_PARTICLES_NUMBER_TAG 3

namespace SimulationUtilities {
	int hash(const Vector3&);
	Vector3 unhash(const int&);	
	int computeProcessID(const Vector3 position, const Vector3 domain_dimension, const int& comm_size);
	int computeProcessID(const int domain_id, const int& comm_size);
	int computeDomainID(const Vector3& position, const Vector3& domain_dimension);

	extern MPI_Comm slave_comm;
	extern int slave_comm_size;
}