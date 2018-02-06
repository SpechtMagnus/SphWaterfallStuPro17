#pragma once
#include "StaticParticle.h"

StaticParticle::StaticParticle() :
	SphParticle(Vector3(), SphParticle::ParticleType::STATIC)
{
}

StaticParticle::StaticParticle(Vector3 position) :
	SphParticle(position, SphParticle::ParticleType::STATIC)
{
}

StaticParticle::~StaticParticle()
{
}