#pragma once
#include "FluidParticle.h"

FluidParticle::FluidParticle() {

}

FluidParticle::FluidParticle(Vector3 position) :
	SphParticle(position) {
}

FluidParticle::FluidParticle(Vector3 position, Vector3 velocity) :
	SphParticle(position, velocity) {
}

FluidParticle::FluidParticle(Vector3 position, Vector3 velocity, double mass) :
	SphParticle(position, velocity, mass){
}

FluidParticle::~FluidParticle() {

}
