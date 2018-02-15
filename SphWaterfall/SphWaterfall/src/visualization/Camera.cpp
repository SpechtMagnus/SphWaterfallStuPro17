#pragma once
#include "Camera.h"
#include "util.h"
#include "Ray.h"
#include <math.h>

Camera::Camera(Vector3 location, Vector3 direction, unsigned int width, unsigned int height, unsigned int ID) {
	this->width = width;
	this->height = height;
	
	this->ID = ID;

	this->location = location;
	this->direction = direction;
}

void Camera::debugRenderFrame(std::vector<DebugObject> particles, int frameID) {

	Frame frame = Frame(this->width, this->height, frameID);
	const double aspectRatio = (double)width / (double)height;
	// view plane parameters
	const double l = -1.f *aspectRatio;   //left
	const double r = +1.f *aspectRatio;   //right
	const double b = -1.f;				  // bottom
	const double t = +1.f;				  // top
	const double d = +2.f;				  // distance to camera


	Vector3 vec_u = findSkalarVectorWithYZero(this->direction).normalize();
	Vector3 vec_v = findUpVector(this->direction, vec_u).normalize();

	for (int x = 0; x < this->width; x++) {
		for (int y = 0; y < this->height; y++) {
			double u = l + (r - l) * (x + 0.5f) / this->width;
			double v = t + (b - t) * (y + 0.5f) / this->height;

			Vector3 vec_s = vec_u * u + vec_v * v + this->direction * d;

			Ray ray = Ray(vec_s.normalize(), this->location);


			frame.setPixel(x, y, castDebugRay(ray, particles));
		}
	}

	this->outputDebugFrame(frame, (("debug_output_cam_" + std::to_string(this->ID)) + ".bmp").c_str());
}

Pixel Camera::castDebugRay(Ray ray, std::vector<DebugObject> particles) {
	double bestDistance = std::numeric_limits<float>::max();
	double waterDepth = 0;
	DebugObject hitObject;
	DebugObject *hit = &hitObject;
	hit = nullptr;

	for (int i = 0; i < particles.size(); i++) {
		DebugObject &obj = particles.at(i);
		double currDist = bestDistance;

		if (obj.intersects(ray, currDist, waterDepth)) {
			if (currDist < bestDistance) {
				bestDistance = currDist;
				hit = &obj;
			}
		}
	}

	return hit == nullptr ? Pixel(0, 0, 0) : Pixel(0, 255, 0);
}

Pixel Camera::castVolumeRay(Ray ray, std::vector<ParticleObject> particles) {
	double bestDistance = std::numeric_limits<float>::max();
	double waterDepth = 0;
	ParticleObject hitObject;
	ParticleObject *hit = &hitObject;
	hit = nullptr;

	Pixel initColor = Pixel(255,255,255); //Make the background white

	for (int i = 0; i < particles.size(); i++) {
		ParticleObject &obj = particles.at(i);
		double currDist = bestDistance;

		if (obj.intersects(ray, currDist, waterDepth)) {
			if (currDist < bestDistance) {
				bestDistance = currDist;
				hit = &obj;
			}
		}
	}
	
	waterDepth = waterDepth - bestDistance;

	if (hit != nullptr) {
		double facRG = 1 - 0.8*exp(-0.2f * waterDepth);
		double facB = 1 - 1.2*exp(-0.1f * waterDepth);
		facB = facB < 0 ? 0 : facB;
		initColor.setRed(initColor.getRedValue() - 255 * facRG);
		initColor.setGreen(initColor.getGreenValue() - 255 * facRG);
		initColor.setBlue(initColor.getBlueValue() - 220 * facB > 30 ? initColor.getBlueValue() - 220 * facB : 30);
		initColor.setShaderUsage(true);
	}


	return initColor;
}

Frame Camera::renderFrame(std::vector<ParticleObject> particles, int frameID) {
	Frame frame = Frame(this->width, this->height, frameID);
	const double aspectRatio = (double)width / (double)height;
	// view plane parameters
	const double l = -1.f *aspectRatio;   //left
	const double r = +1.f *aspectRatio;   //right
	const double b = -1.f;				  // bottom
	const double t = +1.f;				  // top
	const double d = +2.f;				  // distance to camera


	Vector3 vec_u = findSkalarVectorWithYZero(this->direction).normalize();
	Vector3 vec_v = findUpVector(this->direction, vec_u).normalize();

	for (int x = 0; x < this->width; x++) {
		for (int y = 0; y < this->height; y++) {
			double u = l + (r - l) * (x + 0.5f) / this->width;
			double v = t + (b - t) * (y + 0.5f) / this->height;

			Vector3 vec_s = vec_u * u + vec_v * v + this->direction * d;
			Ray ray = Ray(vec_s, this->location);


			frame.setPixel(x, y, castVolumeRay(ray, particles));
		}
	}

	return frame;
}

void Camera::outputDebugFrame(Frame f, const char* fileName) {
	writeFrameToBitmap(f, fileName, this->width, this->height);
}

Vector3 Camera::getLocation() {
	return this->location;
}

Vector3 Camera::getDirection() {
	return this->direction;
}

Frame Camera::getFrame(unsigned int frameID) {
	if (frames.size() >= frameID) {
		return Frame();
		// ^ TODO changend by Magnus
	}
	return frames.at(frameID);
}

//TODO: Impelement Frame Iterator?