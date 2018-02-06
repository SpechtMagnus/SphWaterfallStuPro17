#pragma once
#include "Face.h"


Face::Face() :
	a(Vector3()),
	b(Vector3()),
	c(Vector3())
{
}

Face::Face(Vector3 a, Vector3 b, Vector3 c) :
	a(a), b(b), c(c)
{
}