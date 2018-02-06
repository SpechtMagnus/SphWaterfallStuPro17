#pragma once
#include "FluidParticleGenerator.h"

FluidParticleGenerator::FluidParticleGenerator(double disposalHeight, Vector3 spawn) {
	this-> disposalHeight = disposalHeight;
	this-> spawn = spawn;
}

SphParticle FluidParticleGenerator::spawnParticle() {
	//TODO
	return SphParticle();
}

void FluidParticleGenerator::collectInactive() {
	//TODO
}

