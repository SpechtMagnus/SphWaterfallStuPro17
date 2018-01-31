#pragma once
#include "ISphParticle.h"

class SphParticle : public ISphParticle {
	public:
		SphParticle();
		SphParticle(Vector3);
		SphParticle(Vector3, Vector3);
		~SphParticle();

		virtual ISphParticle::ParticleType getParticleType();

		Vector3 position;
		Vector3 velocity;
	private:
};