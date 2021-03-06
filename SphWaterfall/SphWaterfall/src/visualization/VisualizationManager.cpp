#include "VisualizationManager.h"

void VisualizationManager::init(Vector3 cameraLocation, unsigned int frameWidth, unsigned int frameHeight, int rank) {
	Vector3 cameraDir = Vector3(-cameraLocation.x, -cameraLocation.y / 2, -cameraLocation.z);
	camera = Camera(cameraLocation, cameraDir.normalize(), frameWidth, frameHeight, VisualizationManager::switch_frame);
	if (rank == 0) {
		if (terrain.getVertexCount() > 0 && gate.getVertexCount() > 0) {
			camera.renderGeometryFrames(terrain, gate);
			std::cout << "Terrain rendered" << std::endl;
		}
		else {
			std::cout << "No terrain found" << std::endl;
		}
	}
	camera.shareBaseFrame(rank);
	initilaized = true;
}

void VisualizationManager::debugRenderFrame(std::vector<SphParticle> particles, string fileName) {
	camera.debugRenderFrame(convertSphParticles(particles));
}

void VisualizationManager::importTerrain(Terrain t, bool isGate) {
	if (isGate) { gate = t; }
	else { terrain = t; }
}

void VisualizationManager::setSwitchFrame(int switch_frame)
{
	VisualizationManager::switch_frame = switch_frame;
}

void VisualizationManager::renderFrames(string inputFileName) {
	if (!initilaized) {
		cout << "Please initialize the VisualizationManager before rendering!\n";
	}

	vector<vector<SphParticle>> frameParticles = ParticleIO::importParticles(inputFileName);

	for (int g = 0; g < frameParticles.size(); g++) {
		cout << "Rendering Frame #" << g << "\n";
		vector<ParticleObject> frame = convertFluidParticles(frameParticles.at(g));

		Frame f = camera.renderFrame(frame, g);

		writeFrameToBitmap(f, (("output/frame_" + std::to_string(g)) + ".bmp").c_str(), f.getWidth(), f.getHeight());
	}
}

void VisualizationManager::renderFramesDistributed(string inputFileName, int rank) {
	if (!initilaized) {
		cout << "Please initialize the VisualizationManager before rendering!\n";
	}

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	if (rank == 0) {
		//Send frame count
		vector<vector<SphParticle>> frameParticles = ParticleIO::importParticles(inputFileName);
		for (int i = 1; i < world_size; i++) {
			unsigned int buf[1] =
			{
				static_cast<unsigned int>(frameParticles.size())
			};
			MPI_Send(buf, 1, MPI_UNSIGNED, i, 0, MPI_COMM_WORLD);
		}

		//Send Frames distributed to all other processors
		for (int g = 0; g < frameParticles.size(); g++) {
				int target = (g % (world_size - 1)) + 1;
				vector<ParticleObject> frame = convertFluidParticles(frameParticles.at(g));

				//Send frame size
				unsigned int buf[1] =
				{
					static_cast<unsigned int>(frame.size())
				};
				MPI_Send(buf, 1, MPI_UNSIGNED, target, 0, MPI_COMM_WORLD);

				for (int i = 0; i < frame.size(); i++) {
					ParticleObject::MpiSendPObject(frame[i], target);
				}
		}
	}
	else {
		//Receive Frame count
		unsigned int buf[1];
		MPI_Recv(buf, 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		int frameCount = buf[0];

		vector<vector<ParticleObject>> frameParticles;

		//Receive Frames from master
		for (int g = 0; g < frameCount; g++) {
			if ((g % (world_size - 1)) + 1 == rank) {
				unsigned int frameSize[1];
				MPI_Recv(frameSize, 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				vector<ParticleObject> frame;
				for (int i = 0; i < frameSize[0]; i++) {
					frame.emplace_back(ParticleObject::MpiReceivePObject(0));
				}
				frameParticles.emplace_back(frame);
			}
		}

		int counter = 0;

		//Render frames and save to disk
		for (int i = 0; i < frameCount; i++) {
			if ((i % (world_size - 1)) + 1 == rank) {
				if (counter >= frameParticles.size()) continue;
				vector<ParticleObject> frame = frameParticles.at(counter);

				Frame f = camera.renderFrame(frame, i);

				writeFrameToBitmap(f, (("output/frame_" + std::to_string(i)) + ".bmp").c_str(), f.getWidth(), f.getHeight());
				std::cout << "Frame " << i+1 << "/" << frameCount << " finished on Processor " << rank << "." << std::endl;
				counter++;
			}
		}
	}
}

void VisualizationManager::generateFrames(int frameCount, int particleCount) {
	unordered_map<int, vector<SphParticle>> frames;

	cout << "Generating " << frameCount << " frames...\n";

	for (int i = 1; i <= frameCount; i++) {
		frames.insert_or_assign(i, generateDebugParticles(particleCount));
	}

	cout << "Frames generated\n";

	ParticleIO::exportParticles(frames, "test.particles");
}

vector<SphParticle> VisualizationManager::generateDebugParticles(int count) {
	vector<SphParticle> particles;

	for (int i = 0; i < count; i++) {
		double x = double(rand() % 10000 - 5000) / 1000;
		double y = double(rand() % 10000 - 5000) / 1000;
		double z = double(rand() % 10000 - 5000) / 1000;

		SphParticle f = SphParticle(Vector3(x, y, z), Vector3(0, 0, 0), 1);
		particles.emplace_back(f);
	}
	return particles;
}

bool VisualizationManager::initilaized = false;
Camera VisualizationManager::camera = Camera();
Terrain VisualizationManager::terrain = Terrain();
Terrain VisualizationManager::gate = Terrain();
int VisualizationManager::switch_frame = std::numeric_limits<unsigned int>::max();