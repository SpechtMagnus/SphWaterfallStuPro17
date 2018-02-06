#pragma once
#include "SimulationUtilities.h"

namespace SimulationUtilities {
	MPI_Comm slave_comm;
	int slave_comm_size;

	int SimulationUtilities::hash(const Vector3& vector) {
		int x, y, z;
		x = static_cast<int>(vector.x);
		y = static_cast<int>(vector.y);
		z = static_cast<int>(vector.z);
		y = y << 10;
		z = z << 20;
		return (x + y + z);
	}

	Vector3 SimulationUtilities::unhash(const int& unique_id) {
		int z = unique_id >> 20;
		int y = (unique_id - (z << 20)) >> 10;
		int x = unique_id - (z << 20) - (y << 10);
		return Vector3(x, y, z);
	}

	int SimulationUtilities::computeDomainID(const Vector3 position, const Vector3 domain_dimension) {
		return hash((position / domain_dimension).roundDownward());
	}

	int SimulationUtilities::computeProcessID(const int domain_id, const int comm_size) {
		return abs(domain_id % comm_size);
	}

	int SimulationUtilities::computeProcessID(const Vector3 position, const Vector3 domain_dimension, const int comm_size) {
		return abs(computeDomainID(position, domain_dimension) % comm_size);
	}

}
