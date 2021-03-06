#include "SimulationUtilities.h"

namespace SimulationUtilities {
	MPI_Comm slave_comm;
	int slave_comm_size;

	int hash(const Vector3& vector) {
		int x, y, z;
		x = static_cast<int>(vector.x);
		y = static_cast<int>(vector.y);
		z = static_cast<int>(vector.z);
		y = y << 10;
		z = z << 20;
		return (x + y + z);
	}

	Vector3 unhash(const int& unique_id) {
		int z = unique_id >> 20;
		int y = (unique_id - (z << 20)) >> 10;
		int x = unique_id - (z << 20) - (y << 10);
		return Vector3(x, y, z);
	}

	int computeDomainID(const Vector3& position, const Vector3& domain_dimension) {
		return hash((position / domain_dimension).roundDownward());
	}

	int computeProcessID(const int domain_id) {
		return abs(domain_id % slave_comm_size);
	}

	int computeProcessID(const Vector3 position, const Vector3 domain_dimension) {
		return abs(computeDomainID(position, domain_dimension) % slave_comm_size);
	}

}
