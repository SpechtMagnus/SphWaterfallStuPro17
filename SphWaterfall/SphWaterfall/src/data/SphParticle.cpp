#pragma once
#include "SphParticle.h"

SphParticle::SphParticle() :
	position(Vector3()),
	velocity(Vector3()),
	pType(SphParticle::ParticleType::FLUID)
{
}

SphParticle::SphParticle(Vector3 position) :
	position(position),
	velocity(Vector3()),
	pType(SphParticle::ParticleType::FLUID)
{
}

SphParticle::SphParticle(Vector3 position, Vector3 velocity) :
	position(position),
	velocity(velocity),
	pType(SphParticle::ParticleType::FLUID)
{
}

SphParticle::SphParticle(Vector3 position, Vector3 velocity, double mass) :
	position(position),
	velocity(velocity),
	mass(mass) {
}

SphParticle::SphParticle(Vector3 position, SphParticle::ParticleType particleType) :
	position(position),
	velocity(Vector3()),
	pType(particleType)
{
}

SphParticle::~SphParticle() {

}


bool operator==(const SphParticle a, const SphParticle b)
{
	return ((a.position == b.position) && (a.velocity == b.velocity) && (a.mass == b.mass) && (a.local_density == b.local_density));
}

SphParticle::ParticleType SphParticle::getParticleType()
{
	return this->pType;
}