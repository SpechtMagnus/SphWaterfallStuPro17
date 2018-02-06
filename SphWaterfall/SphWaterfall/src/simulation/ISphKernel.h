#pragma once
#include "../data/Vector3.h"

class ISphKernel {
protected:
	ISphKernel() {}
public:
	~ISphKernel() {}

	virtual double computeKernelValue(const Vector3&) = 0;
	virtual Vector3 computeKernelGradientValue(const Vector3&) = 0;
	virtual double getSmoothingLength() = 0;
private:
};