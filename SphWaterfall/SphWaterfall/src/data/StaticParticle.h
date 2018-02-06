#pragma once
#include "SphParticle.h"

class StaticParticle : public SphParticle {
public:
	StaticParticle();
	StaticParticle(Vector3);
	~StaticParticle();
private:
};