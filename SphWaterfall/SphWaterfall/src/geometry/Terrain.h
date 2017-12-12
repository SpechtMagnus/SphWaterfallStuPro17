#pragma once

#include <vector>

#include "../data/Vector3.h"
#include "Face.h"

class Terrain{
public:
	Terrain(std::vector<Vector3> vertices, std::vector<Vector3> normals, std::vector<int> faces);

	int getVertexCount();
	int getFaceCount();
	Vector3 getVertexPosition(int index);
	int getFaceVertexId(int index, int vertexNr);

private:
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Face> faces;
};