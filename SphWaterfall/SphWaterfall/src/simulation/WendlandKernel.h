#pragma once
#define _USE_MATH_DEFINES

#include <math.h>

#include "../data/Vector3.h"
#include "ISphKernel.h"

//Quintic Wendlandkernel
class WendlandKernel : public ISphKernel {
public:
	WendlandKernel(double aH, double aQMax);
	double computeKernelValue(const Vector3&);
	Vector3 computeKernelGradientValue(const Vector3&);
private:
	double kernel_value_const;
	double kernel_gradient_const;
	const double h;
	const double qMax;
};