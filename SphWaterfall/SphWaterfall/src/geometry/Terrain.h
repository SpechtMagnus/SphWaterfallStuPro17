#pragma once

#include <vector>

#include "../data/Vector3.h"
#include "Face.h"

class Terrain{
public:
	Terrain(std::vector<Vector3> &vertices, std::vector<Vector3> &normals, std::vector<int> &faces);
	Terrain();

	size_t getVertexCount();
	size_t getFaceCount();

	Vector3 getVertex(int);
	Face getFace(int);

private:
	std::vector<Vector3> vertices;
	std::vector<Face> faces;
};