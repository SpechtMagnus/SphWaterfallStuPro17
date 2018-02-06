#pragma once
#include "WendlandKernel.h"

WendlandKernel::WendlandKernel(double aH, double aQMax) :
	h(aH),
	qMax(aQMax)
{

}

double WendlandKernel::computeKernelValue(const Vector3& r) {
	double q = r.length() / qMax;
	if (q >= qMax) {
		return 0.0;
	}

	return 12.0 / (256.0 * M_PI * pow(h, 3.0)) * pow(2 - q, 4.0) * (2.0 * q + 1.0);
}

Vector3 WendlandKernel::computeKernelGradientValue(const Vector3& r) {
	double q = r.length() / qMax;
	if (q >= qMax) {
		return Vector3();
	}

	return 21.0 / (256.0 * M_PI * pow(h, 4.0)) * (-10.0 * q) * pow(2.0 - q, 3.0) * r.normalize();
}

double WendlandKernel::getSmoothingLength() {
	return h;
}
