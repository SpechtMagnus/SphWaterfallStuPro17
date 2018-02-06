#pragma once
#include "SphParticle.h"

class FluidParticle : public SphParticle {
	public:
		FluidParticle();
		FluidParticle(Vector3);
		FluidParticle(Vector3, Vector3);
		FluidParticle(Vector3, Vector3, double);
		~FluidParticle();
		
	private:
};