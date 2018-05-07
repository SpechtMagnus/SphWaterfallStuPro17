#pragma once
#include <iostream>
#include "Frame.h"

//Shader Klasse
class Shader {
public:
	//F�hrt einen Gausschen Weichzeichner angegeben oft durch, mit angegebener Matrix Gr��e
	static Frame applyGaussianSmoothing(Frame f, int shaderDepth, int matrixSize);
};