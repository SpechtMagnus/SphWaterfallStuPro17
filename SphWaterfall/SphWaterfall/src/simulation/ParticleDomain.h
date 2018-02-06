#pragma once
#include "../data/SphParticle.h"
#include "../data/Vector3.h"
#include "../data/NullableWrapper.h"
#include "../data/NullableWrapper.cpp"

#include <vector>
#include <iterator>
#include <unordered_map>
#include <iostream>

class ParticleDomain {
public:
	ParticleDomain();
	ParticleDomain(const Vector3&, const Vector3&);
	~ParticleDomain();

	std::vector<SphParticle> removeParticlesOutsideDomain();

	int size() const;
	const Vector3& getDimensions() const;
	const Vector3& getOrigin() const;
	void addParticle(const SphParticle&);
	std::vector<SphParticle>& getParticles();
	void resetRimParticles();
	void clearRimParticles();
	void setNeighbourRimParticles(const std::unordered_map<int, std::vector<SphParticle>>);
	std::unordered_map<int, std::vector<SphParticle>>& getNeighbourRimParticles();

	std::vector<SphParticle> getTopRimParticles();
	std::vector<SphParticle> getBottomRimParticles();
	std::vector<SphParticle> getLeftRimParticles();
	std::vector<SphParticle> getRightRimParticles();
	std::vector<SphParticle> getFrontRimParticles();
	std::vector<SphParticle> getBackRimParticles();
	std::vector<SphParticle> getFrontBottomRimParticles();
	std::vector<SphParticle> getFrontTopRimParticles();
	std::vector<SphParticle> getBackBottomRimParticles();
	std::vector<SphParticle> getBackTopRimParticles();
	std::vector<SphParticle> getFrontLeftRimParticles();
	std::vector<SphParticle> getFrontRightRimParticles();
	std::vector<SphParticle> getBackLeftRimParticles();
	std::vector<SphParticle> getBackRightRimParticles();
	std::vector<SphParticle> getLeftBottomRimParticles();
	std::vector<SphParticle> getLeftTopRimParticles();
	std::vector<SphParticle> getRightBottomRimParticles();
	std::vector<SphParticle> getRightTopRimParticles();
	std::vector<SphParticle> getFrontLeftBottomRimParticles();
	std::vector<SphParticle> getFrontRightBottomRimParticles();
	std::vector<SphParticle> getFrontLeftTopRimParticles();
	std::vector<SphParticle> getFrontRightTopRimParticles();
	std::vector<SphParticle> getBackLeftBottomRimParticles();
	std::vector<SphParticle> getBackRightBottomRimParticles();
	std::vector<SphParticle> getBackLeftTopRimParticles();
	std::vector<SphParticle> getBackRightTopRimParticles();

private:
	std::vector<SphParticle> particles;
	std::vector<SphParticle> static_particles;
	std::unordered_map<int, std::vector<SphParticle>> neighbour_rim_particles;

	std::vector<int> neighbour_domains;

	NullableWrapper<std::vector<SphParticle>> front_rim_particles;
	NullableWrapper<std::vector<SphParticle>> back_rim_particles;
	NullableWrapper<std::vector<SphParticle>> left_rim_particles;
	NullableWrapper<std::vector<SphParticle>> right_rim_particles;
	NullableWrapper<std::vector<SphParticle>> top_rim_particles;
	NullableWrapper<std::vector<SphParticle>> bottom_rim_particles;
	NullableWrapper<std::vector<SphParticle>> front_bottom_rim_particles;
	NullableWrapper<std::vector<SphParticle>> front_top_rim_particles;
	NullableWrapper<std::vector<SphParticle>> back_bottom_rim_particles;
	NullableWrapper<std::vector<SphParticle>> back_top_rim_particles;
	NullableWrapper<std::vector<SphParticle>> front_left_rim_particles;
	NullableWrapper<std::vector<SphParticle>> front_right_rim_particles;
	NullableWrapper<std::vector<SphParticle>> back_left_rim_particles;
	NullableWrapper<std::vector<SphParticle>> back_right_rim_particles;
	NullableWrapper<std::vector<SphParticle>> left_bottom_rim_particles;
	NullableWrapper<std::vector<SphParticle>> left_top_rim_particles;
	NullableWrapper<std::vector<SphParticle>> right_bottom_rim_particles;
	NullableWrapper<std::vector<SphParticle>> right_top_rim_particles;
	NullableWrapper<std::vector<SphParticle>> front_left_bottom_rim_particles;
	NullableWrapper<std::vector<SphParticle>> front_right_bottom_rim_particles;
	NullableWrapper<std::vector<SphParticle>> front_left_top_rim_particles;
	NullableWrapper<std::vector<SphParticle>> front_right_top_rim_particles;
	NullableWrapper<std::vector<SphParticle>> back_left_bottom_rim_particles;
	NullableWrapper<std::vector<SphParticle>> back_right_bottom_rim_particles;
	NullableWrapper<std::vector<SphParticle>> back_left_top_rim_particles;
	NullableWrapper<std::vector<SphParticle>> back_right_top_rim_particles;
	
	int particles_outside_domain;
	double rim_distance = 2;
	Vector3 origin;
	Vector3 dimensions;
};