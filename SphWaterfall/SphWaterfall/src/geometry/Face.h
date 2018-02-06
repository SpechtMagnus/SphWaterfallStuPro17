#pragma once
#include "../data/Vector3.h"

class Face {
public:
	Face();
	Face(Vector3, Vector3, Vector3);

	const Vector3 a;
	const Vector3 b;
	const Vector3 c;
};