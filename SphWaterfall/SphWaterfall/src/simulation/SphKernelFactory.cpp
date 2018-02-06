#pragma once
#include "SphKernelFactory.h"

SphKernelFactory::SphKernelFactory() {

}

SphKernelFactory::~SphKernelFactory() {

}

ISphKernel* SphKernelFactory::getInstance(int key)
{
	ISphKernel* produced_kernel;

	switch (key)
	{
	default:
		produced_kernel = new WendlandKernel(H, Q_MAX);
		break;
	}

	return produced_kernel;
}