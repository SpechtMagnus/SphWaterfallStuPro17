#pragma once
#include "Camera.h"
#include "util.h"
#include "Ray.h"

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
	const double b = -1.f;   // bottom
	const double t = +1.f;   // top
	const double d = +2.f;   // distance to camera

							//////////////
							// TODO: 2.1.1
							// Cast a ray from 'cameraPos' through the center(!) of each pixel on the view plane.
							// Use the view plane parametrization given above (l, r, b, t and d).
							// cf. lecture slides 39-43

	//Calculate these vectors
	Vector3 vec_u = normalizeVector(findSkalarVectorWithYZero(this->direction));
	Vector3 vec_v = normalizeVector(findUpVector(this->direction, vec_u));

	for (int x = 0; x < this->width; x++) {
		for (int y = 0; y < this->height; y++) {
			double u = l + (r - l) * (x + 0.5f) / this->width;
			double v = t + (b - t) * (y + 0.5f) / this->height;

			Vector3 vec_s = Vector3(vec_u.x * u + vec_v.x * v + this->direction.x * d,
				vec_u.y * u + vec_v.y * v + this->direction.y * d,
				vec_u.z * u + vec_v.z * v + this->direction.z * d);

			Ray ray = Ray(vec_s, this->location);
			

			frame.setPixel(x, y, castDebugRay(ray, particles));
		}
	}

	this->outputDebugFrame(frame, (("debug_output_cam_" + std::to_string(this->ID)) + ".bmp").c_str());
}

Pixel Camera::castDebugRay(Ray ray, std::vector<DebugObject> particles) {
	double bestDistance = std::numeric_limits<float>::max();
	DebugObject hitObject;
	DebugObject *hit = &hitObject;
	hit = nullptr;

	for (int i = 0; i < particles.size(); i++) {
		DebugObject &obj = particles.at(i);
		double currDist = bestDistance;

		if (obj.intersects(ray, currDist)) {
			if (currDist < bestDistance) {
				bestDistance = currDist;
				hit = &obj;
			}
		}
	}

	return hit == nullptr ? Pixel(0, 0, 0) : hitObject.getColor();
}

void Camera::renderFrame(std::vector<SphParticle> particles) {
	//TODO: implement rendering algorithm
}

void Camera::mergeFramesAndFlushVideo(std::string file) {
	//TODO: merge all Frames to a video
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