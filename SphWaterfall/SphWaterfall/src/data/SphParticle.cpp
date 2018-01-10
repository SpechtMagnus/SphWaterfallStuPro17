#pragma once
#include "SphParticle.h"

SphParticle::SphParticle() {

}

SphParticle::SphParticle(Vector3 position) {
	this->position = position;
}

SphParticle::SphParticle(Vector3 position, Vector3 velocity) :
	position(position),
	velocity(velocity) {
}

SphParticle::~SphParticle() {

}
