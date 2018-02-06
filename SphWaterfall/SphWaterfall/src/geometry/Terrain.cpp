#pragma once

#include <vector>

#include "Terrain.h"


Terrain::Terrain(std::vector<Vector3> &vertices, std::vector<Vector3> &normals, std::vector<int> &faces) :
	vertices(vertices)
{
	std::vector<int>::iterator it;
	for (it = faces.begin(); it != faces.end(); it++) {
		this->faces.push_back(Face::Face(vertices[*it], vertices[*++it], vertices[*++it]));
	}
}

Terrain::Terrain() :
	vertices(std::vector<Vector3>()),
	faces(std::vector<Face>())
{
}


size_t Terrain::getVertexCount()
{
	return this->vertices.size();
}

size_t Terrain::getFaceCount()
{
	return this->faces.size();
}


Vector3 Terrain::getVertex(int index)
{
	if (index < 0 || index >= this->vertices.size())
		return Vector3::Vector3();

	return this->vertices[index];
}

Face Terrain::getFace(int faceId)
{
	if (faceId < 0 || faceId >= this->faces.size())
		return Face();

	return this->faces[faceId];
}
