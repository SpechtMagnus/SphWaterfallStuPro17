#pragma once

#include "mpi.h";
#include "../data/Vector3.h";
#include "../data/SphParticle.h";

// Smoothing length
#define H 1.0
// Influential radius
#define Q_MAX 2.0

namespace SimulationUtilities {
	int hash(const Vector3&);
	Vector3 unhash(const int&);	
	int computeProcessID(const Vector3 position, const Vector3 domain_dimension, const int comm_size);
	int computeProcessID(const int domain_id, const int comm_size);
	int computeDomainID(const Vector3 position, const Vector3 domain_dimension);

	extern MPI_Comm slave_comm;
	extern int slave_comm_size;
}