#pragma once
#include "ISphParticle.h"

class SphParticle : public ISphParticle {
	public:
		SphParticle();
		SphParticle(Vector3);
		SphParticle(Vector3, Vector3);
		SphParticle(Vector3, Vector3, double);
		~SphParticle();

		void setDensity(double);
		void setViscosity(double);

		Vector3 position;
		Vector3 velocity;
		double mass;
		double density;
		double viscosity;
	private:
};