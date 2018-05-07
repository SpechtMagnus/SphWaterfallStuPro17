#include "Frame.h"
#include <iostream>


Frame::Frame() {
	this->width = 0;
	this->height = 0;
}

Frame::Frame(unsigned int width, unsigned int height) {
	this->width = width;
	this->height = height;
	for (int i = 0; i < this->width*this->height; i++) {
		this->pixels.emplace_back(Pixel(255, 255, 255));
	}
}

Pixel& Frame::getPixel(unsigned int x, unsigned int y) {
    auto p = Pixel(0, 0, 0);
	if (x >= this->width || y >= this->height) return p;
	return pixels.at(y*width + x);
}

void Frame::setPixel(unsigned int x, unsigned int y, Pixel pixel) {
	if (x >= this->width || y >= this->height) return;
	this->pixels[y*width + x] = pixel;
}

unsigned int Frame::getWidth() {
	return this->width;
}

unsigned int Frame::getHeight() {
	return this->height;
}

void Frame::MpiSendFrame(Frame frame, int dest)
{
	//Send frame size
	unsigned int buf[2] = { frame.getWidth(), frame.getHeight() };
	MPI_Send(buf, 2, MPI_UNSIGNED, dest, 0, MPI_COMM_WORLD);

	//Send frame pixel by pixel
	for (int x = 0; x < frame.getWidth(); x++) {
		for (int y = 0; y < frame.getHeight(); y++) {
			unsigned short pixel[3] = {
				frame.getPixel(x, y).getRedValue(),		//R
				frame.getPixel(x, y).getGreenValue(),	//G
				frame.getPixel(x, y).getBlueValue(),	//B
			};
			MPI_Send(pixel, 3, MPI_UNSIGNED_SHORT, dest, 0, MPI_COMM_WORLD);
			
			double pixel_depth[1] = { frame.getPixel(x, y).getBaseDepth() };
			MPI_Send(pixel_depth, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
		}
	}

}

Frame Frame::MpiReceiveFrame(int source)
{
	unsigned int buf[2];
	MPI_Recv(buf, 2, MPI_UNSIGNED, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	Frame frame = Frame(buf[0], buf[1]);

	for (int x = 0; x < frame.getWidth(); x++) {
		for (int y = 0; y < frame.getHeight(); y++) {
			unsigned short pixel[3];
			MPI_Recv(pixel, 3, MPI_UNSIGNED_SHORT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			frame.setPixel(x, y, Pixel(pixel[0], pixel[1], pixel[2]));

			
			double pixel_depth[1];
			MPI_Recv(pixel_depth, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			frame.getPixel(x, y).setBaseDepth(pixel_depth[0]);
		}
	}

	return frame;
}

