#pragma once
#include "SphManager.h"

SphManager::SphManager(const Vector3& domain_dimensions, double simulation_time, double timestep_duration) :
	domain_dimensions(domain_dimensions),
	simulation_time(simulation_time),
	timestep_duration(timestep_duration) 
{
	kernel = kernel_factory.getInstance(1);
	neighbour_search = neighbour_search_factory.getInstance(1);
}

SphManager::~SphManager() {

}

void SphManager::simulate() {
	double remaining_simulation_time = simulation_time;
	while (remaining_simulation_time > 0) {
		exchangeRimParticles();
		std::cout << "after exchange rim particles" << std::endl;
		update(timestep_duration);
		std::cout << "after update" << std::endl;
		exchangeParticles();
		std::cout << "after exchange particles" << std::endl;
		remaining_simulation_time -= timestep_duration;
	}
}

void SphManager::update(double timestep_duration) {
	// neighbour search
	std::vector<SphParticle> each_neighbour_particles;
	std::unordered_map <int, std::vector<SphParticle>> each_neighbour_rim_particles;
	int i = 0;
	for (auto& each_domain : domains) {
		std::cout << "before get neighbour rim particles" << std::endl;
		each_neighbour_rim_particles = each_domain.second.getNeighbourRimParticles();
		for (auto& each_particle : each_domain.second.getParticles()) {
			each_neighbour_particles = each_domain.second.getParticles();
			for (auto domain_id : neighbour_search->findRelevantNeighbourDomains(each_particle, domain_dimensions)) {
				std::cout << domain_id << std::endl;
				each_neighbour_particles.insert(each_neighbour_particles.end(),
					each_neighbour_rim_particles.at(domain_id).begin(),
					each_neighbour_rim_particles.at(domain_id).end());
				std::cout << "after find relevant neighbour domain222222" << std::endl;
			}
			each_neighbour_particles = neighbour_search->findNeigbours(each_particle, each_neighbour_particles);
			neighbour_particles[i] = std::pair<SphParticle, std::vector<SphParticle>>(each_particle, each_neighbour_particles);
			i++;
		}
	}
	std::cout << "after neighbour search" << std::endl;
	// compute and set local densities
	for (auto& each_domain : domains) {
		for (auto& each_particle : each_domain.second.getParticles()) {
			computeLocalDensity(each_particle);
		}
	}
	// compute and update Velocities and position
	for (auto& each_domain : domains) {
		for (auto& each_particle : each_domain.second.getParticles()) {
			std::cout << each_particle.position << std::endl;
			updateVelocity(each_particle, timestep_duration);
		}
	}
}

void SphManager::updateVelocity(SphParticle& particle, double timestep_duration) {
	Vector3 accelleration_timestep_start = computeAcceleration(particle);

	//std::cout << "after compute first acceleration" << std::endl; //debug

	particle.velocity = particle.velocity + ((timestep_duration / 2) * accelleration_timestep_start);
	Vector3 position_timestep_half = particle.position + ((timestep_duration / 2) * particle.velocity);

	Vector3 accelleration_timestep_half = computeAcceleration(particle);

	//std::cout << "after compute second acceleration" << std::endl; //debug

	Vector3 velocity_timestep_end = particle.velocity + (timestep_duration * accelleration_timestep_half);
	particle.position = position_timestep_half + ((timestep_duration / 2) * velocity_timestep_end);
}

Vector3 SphManager::computeAcceleration(SphParticle& particle) {
	Vector3 acceleration;
	Vector3 gravity_acceleration = Vector3(0, -9.81, 0);
	acceleration = gravity_acceleration + computeDensityAcceleration(particle) + computeViscosityAcceleration(particle);
	return acceleration;
}

void SphManager::computeLocalDensity(SphParticle& particle) {
	double local_density = 0;
	// std::vector<SphParticle> neighbours = neighbour_particles.at(particle); TODO: implement Hash for SphParticle (operator())
	std::vector<SphParticle> neighbours;
	for (auto neighbour : neighbour_particles) {
		if (particle == neighbour.second.first) {
			neighbours = neighbour.second.second;
		}
	}

	for (SphParticle neighbour_particle : neighbours) {
		local_density += neighbour_particle.mass * kernel->computeKernelValue(particle.position - neighbour_particle.position);
	}

	particle.local_density = local_density;
}

Vector3 SphManager::computeDensityAcceleration(SphParticle& particle) {
	// std::vector<SphParticle> neighbours = neighbour_particles.at(particle); TODO: implement Hash for SphParticle (operator())
	std::vector<SphParticle> neighbours;
	for (auto neighbour : neighbour_particles) {
		if (particle == neighbour.second.first) {
			neighbours = neighbour.second.second;
		}
	}

	Vector3 density_acceleration = Vector3();
	double particle_local_pressure = computeLocalPressure(particle);

	for (SphParticle neighbour_particle : neighbours) {
		density_acceleration += neighbour_particle.mass * 
			((computeLocalPressure(neighbour_particle) / (neighbour_particle.local_density * neighbour_particle.local_density)) + 
			(particle_local_pressure / (particle.local_density * particle.local_density))) * 
			kernel->computeKernelGradientValue(particle.position - neighbour_particle.position);
	}

	//std::cout << "after compute density acceleration" << std::endl; //debug

	return density_acceleration;
}

double SphManager::computeLocalPressure(SphParticle& particle) {
	double local_pressure = 0.0;
	double refrence_density_of_water = 1.0;
	//double pressure_constant = 1.0; //to be evaluated, is chosen arbitrarily at the moment
	double reference_pressure = 101300.0; //not sure atm

	//local_pressure = pressure_constant *(particle.local_density - refrence_density_of_water); //old version meight be better
	local_pressure = reference_pressure * (std::pow((particle.local_density / refrence_density_of_water), 7.0) - 1); //other implementation of local pressure
	//std::cout << local_pressure << " " << (std::pow((particle.local_density / refrence_density_of_water), 7.0) - 1) << std::endl; //debug
	return local_pressure;
}

Vector3 SphManager::computeViscosityAcceleration(SphParticle& particle) {
	// std::vector<SphParticle> neighbours = neighbour_particles.at(particle); TODO: implement Hash for SphParticle (operator())
	std::vector<SphParticle> neighbours;
	for (auto neighbour : neighbour_particles) {
		if (particle == neighbour.second.first) {
			neighbours = neighbour.second.second;
		}
	}
	Vector3 viscosity_acceleration = Vector3();

	for (SphParticle neighbour_particle : neighbours)
	{
		Vector3 rij = neighbour_particle.position - particle.position;
		viscosity_acceleration += neighbour_particle.mass * ((4 * 1.0 * rij * kernel->computeKernelGradientValue(rij)) / 
			((particle.local_density + neighbour_particle.local_density) * (rij.length() * rij.length()))) * 
			(particle.velocity - neighbour_particle.velocity);
	}

	//std::cout << "after compute viscosity acceleration" << std::endl; //debug

	if (particle.local_density == 0) {
		return Vector3();
	}
	return ((1 / particle.local_density) * viscosity_acceleration);
}

void SphManager::findNeighbourDomains(ParticleDomain) {

}

void SphManager::exchangeRimParticles() {
	// target domain id, source domain id, rim particles from source in direction of target domain
	std::unordered_map<int, std::unordered_map<int, std::vector<SphParticle>>> target_map;

	for (auto each_domain : domains) {
		int source_id = each_domain.first;
		Vector3 index_coords = unhash(source_id);

		each_domain.second.clearRimParticles();

		int target_id = hash(index_coords + Vector3(-1, -1, -1));
		target_map[target_id][source_id] = each_domain.second.getFrontLeftBottomRimParticles();
		target_id = hash(index_coords + (Vector3(-1, -1, 0)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getLeftBottomRimParticles();
		target_id = hash(index_coords + (Vector3(-1, -1, 1)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getBackLeftBottomRimParticles();
		target_id = hash(index_coords + (Vector3(-1, 0, -1)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getFrontLeftRimParticles();
		target_id = hash(index_coords + (Vector3(-1, 0, 0) * domain_dimensions));
		target_map[target_id][source_id] = each_domain.second.getLeftRimParticles();
		target_id = hash(index_coords + (Vector3(-1, 0, 1)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getBackLeftRimParticles();
		target_id = hash(index_coords + (Vector3(-1, 1, -1)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getFrontLeftTopRimParticles();
		target_id = hash(index_coords + (Vector3(-1, 1, 0)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getLeftTopRimParticles();
		target_id = hash(index_coords + (Vector3(-1, 1, 1)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getBackLeftTopRimParticles();
		target_id = hash(index_coords + (Vector3(0, -1, -1)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getFrontBottomRimParticles();
		target_id = hash(index_coords + (Vector3(0, -1, 0)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getBottomRimParticles();
		target_id = hash(index_coords + (Vector3(0, -1, 1)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getBackBottomRimParticles();
		target_id = hash(index_coords + (Vector3(0, 0, -1)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getFrontRimParticles();
		target_id = hash(index_coords + (Vector3(0, 0, 1)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getBackRimParticles();
		target_id = hash(index_coords + (Vector3(0, 1, -1)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getFrontTopRimParticles();
		target_id = hash(index_coords + (Vector3(0, 1, 0)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getTopRimParticles();
		target_id = hash(index_coords + (Vector3(0, 1, 1)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getBackTopRimParticles();
		target_id = hash(index_coords + (Vector3(1, -1, -1)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getFrontRightBottomRimParticles();
		target_id = hash(index_coords + (Vector3(1, -1, 0)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getRightBottomRimParticles();
		target_id = hash(index_coords + (Vector3(1, -1, 1)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getBackRightBottomRimParticles();
		target_id = hash(index_coords + (Vector3(1, 0, -1)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getFrontRightRimParticles();
		target_id = hash(index_coords + (Vector3(1, 0, 0)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getRightRimParticles();
		target_id = hash(index_coords + (Vector3(1, 0, 1)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getBackRightRimParticles();
		target_id = hash(index_coords + (Vector3(1, 1, -1)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getFrontRightTopRimParticles();
		target_id = hash(index_coords + (Vector3(1, 1, 0)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getRightTopRimParticles();
		target_id = hash(index_coords + (Vector3(1, 1, 1)) * domain_dimensions);
		target_map[target_id][source_id] = each_domain.second.getBackRightTopRimParticles();
	}

	std::unordered_map<int, std::unordered_map<int, std::vector<SphParticle>>> new_rim_particles;
	std::vector<SphParticle> incoming_rim_particles;
	int rank;
	MPI_Comm_rank(slave_comm, &rank);
	int count = 1;

	for (auto target : target_map) {
		for (auto rim_particles : target.second) {
			int target_process_id = computeProcessID(target.first, slave_comm_size);
			if (rim_particles.second.empty()) {
				if (target_process_id == rank) {
					new_rim_particles[target.first][rim_particles.first] = rim_particles.second;
				}
				else {
					// target domain id, source domain id, tag der richtigen Nachricht
					std::array<int, 3> meta = { target.first, rim_particles.first, count };
					MPI_Request request;
					MPI_Isend(meta.data(), meta.size() * sizeof(int), MPI_BYTE, target_process_id, 0, slave_comm, &request);
					MPI_Request_free(&request);
					std::cout << rim_particles.second.data() << std::endl; // debug
					// send particles
					request;
					MPI_Isend(rim_particles.second.data(), rim_particles.second.size() * sizeof(SphParticle), MPI_BYTE, target_process_id, count, slave_comm, &request);
					MPI_Request_free(&request);

					// increment unique tag
					count++;
				}
			}
		}	
	}

	MPI_Barrier(slave_comm);

	// receive until there is nothing left
	int flag, source, tag;
	MPI_Status status;
	MPI_Iprobe(MPI_ANY_SOURCE, 0, slave_comm, &flag, &status);

	while (flag) {
		//std::cout << "exchange rim particles in flag loop" << std::endl;
		source = status.MPI_SOURCE;
		//std::cout << "exchange rim particles2" << std::endl;
		tag = status.MPI_TAG;
		//std::cout << tag;

		MPI_Get_count(&status, MPI_BYTE, &count);
		std::array<int, 3> meta = std::array<int, 3>();
		MPI_Recv(meta.data(), count, MPI_BYTE, source, tag, slave_comm, &status);

		MPI_Probe(source, meta[2], slave_comm, &status);
		MPI_Get_count(&status, MPI_BYTE, &count);

		incoming_rim_particles = std::vector<SphParticle>(count);
		source = status.MPI_SOURCE;
		tag = status.MPI_TAG;
		std::cout << count << std::endl; //debug
		MPI_Recv(incoming_rim_particles.data(), count, MPI_BYTE, source, tag, slave_comm, &status);

		new_rim_particles[meta[0]][meta[1]] = incoming_rim_particles;
		// next message
		MPI_Iprobe(MPI_ANY_SOURCE, 0, slave_comm, &flag, &status);
	}

	for (auto target : new_rim_particles) {
		if (domains.count(target.first) != 0) {
			getParticleDomain(target.first).setNeighbourRimParticles(target.second);
		}
	}

	MPI_Barrier(slave_comm);
}


void SphManager::exchangeParticles() {
	std::unordered_map<int, std::vector<SphParticle>> target_map;
	std::vector<SphParticle> all_new_particles;
	std::vector<SphParticle> incoming_particles;

	for (auto each_domain : domains) {
		std::vector<SphParticle> outside_particles = each_domain.second.removeParticlesOutsideDomain();
		for (SphParticle each_particle : outside_particles) {
			int target_id = computeProcessID(each_particle.position, domain_dimensions, slave_comm_size);
			int count = target_map.count(target_id);

			if (count == 0) {
				target_map[target_id] = std::vector<SphParticle>();
			}

			std::vector<SphParticle> list = target_map.at(target_id);
			list.push_back(each_particle);
		}
	}

	// don't send to yourself
	int rank;
	MPI_Comm_rank(slave_comm, &rank);
	int count = target_map.count(rank);
	if (count != 0) {
		all_new_particles = target_map.at(rank);
		target_map.erase(rank);
	}

	for (auto vector : target_map) {
		MPI_Request request;
		MPI_Isend(vector.second.data(), vector.second.size() * sizeof(SphParticle), MPI_BYTE, vector.first, 0, slave_comm, &request);
		MPI_Request_free(&request);
	}

	MPI_Barrier(slave_comm);

	// receive until there is nothing left
	int flag;
	MPI_Status status;
	MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, slave_comm, &flag, &status);

	while (flag) {
		int source = status.MPI_SOURCE;
		int tag = status.MPI_TAG;
		int count;

		MPI_Get_count(&status, MPI_BYTE, &count);

		incoming_particles = std::vector<SphParticle>(count);

		MPI_Recv(incoming_particles.data(), count, MPI_BYTE, source, tag, slave_comm, &status);
		std::move(incoming_particles.begin(), incoming_particles.end(), std::inserter(all_new_particles, all_new_particles.end()));

		// next message
		MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, slave_comm, &flag, &status);
	}

	add_particles(all_new_particles);

	MPI_Barrier(slave_comm);
}

ParticleDomain& SphManager::getParticleDomain(const int& unique_id) {
	int count = domains.count(unique_id);
	if (count == 0) {
		domains[unique_id] = ParticleDomain(unhash(unique_id), domain_dimensions);
	}
	return domains.at(unique_id);
}

void SphManager::add_particles(const std::vector<SphParticle>& new_particles) {
	for (SphParticle particle : new_particles) {
		int domain_id = computeDomainID(particle.position, domain_dimensions);
		//std::cout << domain_id << std::endl; // Debug output
		getParticleDomain(domain_id).addParticle(particle);
		//std::cout << particle.position << std::endl; // debug output
	}
}

MPI_Request SphManager::requestRimParticles(const Vector3& neighbourDomain, const Vector3& source) {
	int domain_id = hash(neighbourDomain);
	int request_id = hash(source);

	MPI_Request request;
	MPI_Isend(&request_id, 1, MPI_INT, domain_id, request_id % slave_comm_size, slave_comm, &request);
	return request;
}

void SphManager::sendRimParticles(const int& destination, const int& requester) {
	Vector3 request_coords = unhash(requester);
	Vector3 target_coords = unhash(destination);
	ParticleDomain destination_domain = getParticleDomain(destination);

	std::vector<SphParticle> rim_particles;

	// if you know how to do better ... do it, cause this sucks

	if (request_coords.x > target_coords.x) {
		if (request_coords.y > target_coords.y) {
			if (request_coords.z > target_coords.z) {
				rim_particles = destination_domain.getFrontRightTopRimParticles();
			}
			else if (request_coords.z < target_coords.z) {
				rim_particles = destination_domain.getFrontRightBottomRimParticles();
			}
			else {
				rim_particles = destination_domain.getFrontRightRimParticles();
			}
		}
		else if (request_coords.y < target_coords.y) {
			if (request_coords.z > target_coords.z) {
				rim_particles = destination_domain.getFrontLeftTopRimParticles();
			}
			else if (request_coords.z < target_coords.z) {
				rim_particles = destination_domain.getFrontLeftBottomRimParticles();
			}
			else {
				rim_particles = destination_domain.getFrontLeftRimParticles();
			}
		}
		else {
			if (request_coords.z > target_coords.z) {
				rim_particles = destination_domain.getFrontTopRimParticles();
			}
			else if (request_coords.z < target_coords.z) {
				rim_particles = destination_domain.getFrontBottomRimParticles();
			}
			else {
				rim_particles = destination_domain.getFrontRimParticles();
			}
		}
	}
	else if (request_coords.x < target_coords.x) {
		if (request_coords.y > target_coords.y) {
			if (request_coords.z > target_coords.z) {
				rim_particles = destination_domain.getBackRightTopRimParticles();
			}
			else if (request_coords.z < target_coords.z) {
				rim_particles = destination_domain.getBackRightBottomRimParticles();
			}
			else {
				rim_particles = destination_domain.getBackRightRimParticles();
			}
		}
		else if (request_coords.y < target_coords.y) {
			if (request_coords.z > target_coords.z) {
				rim_particles = destination_domain.getBackLeftTopRimParticles();
			}
			else if (request_coords.z < target_coords.z) {
				rim_particles = destination_domain.getBackLeftBottomRimParticles();
			}
			else {
				rim_particles = destination_domain.getBackLeftRimParticles();
			}
		}
		else {
			if (request_coords.z > target_coords.z) {
				rim_particles = destination_domain.getBackTopRimParticles();
			}
			else if (request_coords.z < target_coords.z) {
				rim_particles = destination_domain.getBackBottomRimParticles();
			}
			else {
				rim_particles = destination_domain.getBackRimParticles();
			}
		}
	} 
	else if (request_coords.y > target_coords.y) {
		if (request_coords.z > target_coords.z) {
			rim_particles = destination_domain.getRightTopRimParticles();
		}
		else if (request_coords.z < target_coords.z) {
			rim_particles = destination_domain.getRightBottomRimParticles();
		}
		else {
			rim_particles = destination_domain.getRightRimParticles();
		}
	}
	else if (request_coords.y < target_coords.y) {
		if (request_coords.z > target_coords.z) {
			rim_particles = destination_domain.getLeftTopRimParticles();
		}
		else if (request_coords.z < target_coords.z) {
			rim_particles = destination_domain.getLeftBottomRimParticles();
		}
		else {
			rim_particles = destination_domain.getLeftRimParticles();
		}
	} 
	else if (request_coords.z > target_coords.z) {
		rim_particles = destination_domain.getTopRimParticles();
	}
	else if (request_coords.z < target_coords.z) {
		rim_particles = destination_domain.getBottomRimParticles();
	}

	MPI_Request request;
	MPI_Isend(rim_particles.data(), rim_particles.size() * sizeof(SphParticle), MPI_BYTE, requester % slave_comm_size, 0, slave_comm, &request);
	MPI_Request_free(&request);
}
