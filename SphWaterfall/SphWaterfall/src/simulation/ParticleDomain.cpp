#include "ParticleDomain.h"

ParticleDomain::ParticleDomain() :
	origin(Vector3()),
	dimensions(Vector3()),
	number_of_fluid_particles(0),
	has_static_particles(false)
{
	particles = std::vector<SphParticle>();
}

ParticleDomain::ParticleDomain(const Vector3& origin, const Vector3& dimension) : 
	origin(origin),
	dimensions(dimension),
	number_of_fluid_particles(0),
	has_static_particles(false)
{
	particles = std::vector<SphParticle>();
}

ParticleDomain::~ParticleDomain() {

}

int ParticleDomain::size() const {
	return particles.size();
}

void ParticleDomain::clearParticles() {
	particles.clear();
	has_static_particles = false;
	number_of_fluid_particles = 0;
}

void ParticleDomain::clearParticles(SphParticle::ParticleType particle_type) {
	for (int i = 0; i < particles.size(); i++) {
		SphParticle& each_particle = particles.at(i);
		if (each_particle.getParticleType() == particle_type) {
			particles.erase(particles.begin() + i);
			i--;
		}
	}
	if (particle_type == SphParticle::STATIC) {
		has_static_particles = false;
	}
	else if (particle_type == SphParticle::FLUID) {
		number_of_fluid_particles = 0;
	}
}

void ParticleDomain::clearNeighbourRimParticles() {
	neighbour_rim_particles.clear();
}

void ParticleDomain::clearNeighbourRimParticles(SphParticle::ParticleType particle_type) {
	for (auto& each_rim : neighbour_rim_particles) {
		for (int i = 0; i < each_rim.second.size(); i++) {
			SphParticle& each_particle = each_rim.second.at(i);
			if (each_particle.getParticleType() == particle_type) {
				each_rim.second.erase(each_rim.second.begin() + i);
				--i;
			}
		}
	}
}

std::vector<SphParticle> ParticleDomain::removeParticlesOutsideDomain(double sink_height) {
	std::vector<SphParticle> outside_particles;
	int domain_id = SimulationUtilities::computeDomainID(origin, dimensions);

	int particle_index = 0;
	while (particle_index < particles.size()) {
		SphParticle each_particle = particles.at(particle_index);
		if (each_particle.getParticleType() == SphParticle::FLUID) {
			if (each_particle.position.y <= sink_height) {
				//Find particles below sink
				particles.erase(particles.begin() + particle_index);
				particle_index--;
				std::cout << "-- Particle below sink height deleted." << std::endl;
			}
			else if (SimulationUtilities::computeDomainID(each_particle.position, dimensions) != domain_id) {
				//std::cout << "outside particle: " << each_particle << " origin: " << origin << "  dimension: " << dimensions << std::endl << "debug: " << vector_difference << std::endl; //debug
				//Find particles outside domain
				outside_particles.push_back(each_particle);
				particles.erase(particles.begin() + particle_index);
				particle_index--;
			}
			else {
				particle_index++;
			}
		}
		else {
			particle_index++;
		}
	}

	return outside_particles;
}

void ParticleDomain::addNeighbourRimParticles(const std::unordered_map<int, std::vector<SphParticle>>& neighbour_rim_map) {
	for (auto& each_neighbour_particles : neighbour_rim_map) {
		neighbour_rim_particles[each_neighbour_particles.first].insert(neighbour_rim_particles[each_neighbour_particles.first].end(), each_neighbour_particles.second.begin(), each_neighbour_particles.second.end());
	}
}

std::unordered_map<int, std::vector<SphParticle>>& ParticleDomain::getNeighbourRimParticles() {
	return neighbour_rim_particles;
}

const Vector3& ParticleDomain::getDimensions() const {
	return dimensions;
}

const Vector3& ParticleDomain::getOrigin() const {
	return origin;
}

void ParticleDomain::addParticle(const SphParticle& particle) {
	if (particle.getParticleType() == SphParticle::FLUID) {
		number_of_fluid_particles++;
	}
	if (!has_static_particles && particle.getParticleType() == SphParticle::STATIC) {
		has_static_particles = true;
	}
	particles.push_back(particle);
}

std::vector<SphParticle>& ParticleDomain::getParticles() {
	return particles;
}

const bool ParticleDomain::hasFluidParticles() const {
	return number_of_fluid_particles != 0;
}

const bool& ParticleDomain::hasStaticParticles() const {
	return has_static_particles;
}

std::unordered_map<int, std::vector<SphParticle>> ParticleDomain::getRimParticleTargetMap(SphParticle::ParticleType particle_type) {
	std::unordered_map<int, std::vector<SphParticle>> target_map;
	int domain_id = SimulationUtilities::computeDomainID(origin, dimensions);
	for (SphParticle& particle : particles) {
		if (particle.getParticleType() == particle_type) {
			for (int x = -1; x <= 1; x++) {
				for (int y = -1; y <= 1; y++) {
					for (int z = -1; z <= 1; z++) {
						int id = SimulationUtilities::computeDomainID(particle.position + ((Vector3(x, y, z).normalize() * R_MAX)), dimensions);
						if (id != domain_id) {
							target_map[id].push_back(particle);
						}
					}
				}
			}
		}
	}

	return target_map;
}
