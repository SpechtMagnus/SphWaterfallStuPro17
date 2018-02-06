#pragma once
#include "StaticParticleGenerator.h"
#include "../data/FluidParticle.h"

StaticParticleGenerator::StaticParticleGenerator() {

}

std::vector<SphParticle> StaticParticleGenerator :: generateStaticParticles(Terrain source) {
	std::vector<SphParticle> staticParticles;
	for (int i = 0; i < source.getVertexCount(); i++) {
		staticParticles.push_back(FluidParticle(source.getVertexPosition(i)));
		
	}
	return staticParticles;

}

/*
 * Generates static particles uniformly on a given face
 * face: The face to genrate the particles on
 * particleDensity: How much space lies between 2 particles
 */
std::vector<ISphParticle> StaticParticleGenerator::generateStaticParticles(Face face, float particleDensity)
{
	std::vector<ISphParticle> result = std::vector<ISphParticle>();
	Vector3 particlePosition = Vector3();
	
	//Create uniform grid of particles
	for (double x = 0.0; x <= 1.0; x += particleDensity)
	{
		for (double y = 0.0; y <= 1.0; y += particleDensity)
		{
			if (x + y < 1)
				continue;

			particlePosition = face.a + (face.b * x) + (face.c * y);
		}
	}
}