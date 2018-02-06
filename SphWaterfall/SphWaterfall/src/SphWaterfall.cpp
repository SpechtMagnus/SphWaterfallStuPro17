#pragma once
#include <thread>

#include "mpi.h"

#include "cui/CUI.h"
//#include "simulation/SimulationUtilities.h"
#include "data\FluidParticle.h"

void loadMesh() {
	cout << "command is loadMesh" << endl;
}

void generateParticle() {
	cout << "command is particleGen" << endl;
}

void moveShutter() {
	cout << "command is moveShutter" << endl;
}

void simulate() {
	cout << "command is simulate" << endl;

	SphManager sph_manager = SphManager(Vector3(2, 2, 2), 5, 1);
	std::vector<SphParticle> particles;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			for (int k = 0; k < 5; k++) {
				SphParticle particle = FluidParticle(Vector3(i, j, k), Vector3(), 1.0);
				particles.push_back(particle);
				//cout << particle.position << endl;
			}
		}
	}
	sph_manager.add_particles(particles);
	sph_manager.simulate();
}

void render() {
	cout << "command is render" << endl;
}

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	bool exit_programm = false;
	std::thread cuiThread;
	int *command_buffer;
	command_buffer = (int*)malloc(sizeof(int));

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int color = 1337;
	if (rank == 0) {
		color = MPI_UNDEFINED;
	}

	MPI_Comm_split(MPI_COMM_WORLD, color, 0, &slave_comm);
	if (rank != 0) {
		MPI_Comm_size(slave_comm, &slave_comm_size);
	}

	while (!exit_programm) {
		if (rank == 0) {
			cuiThread = std::thread(CUI::readCommand, command_buffer);
			cuiThread.join();
		}

		MPI_Bcast(command_buffer, 1, MPI_INT, 0, MPI_COMM_WORLD);

		if (*command_buffer == 0) {
			exit_programm = true;
		} 
		else if (rank != 0) {
			cout << "command buffer is " << command_buffer[0] << ". ";

			if (*command_buffer == 1) {
				loadMesh();
				cout << "mesh loading finished from processor " << rank << " out of " << slave_comm_size << " processors" << endl;
			} 
			else if (*command_buffer == 2) {
				generateParticle();
				cout << "particle generation finished from processor " << rank << " out of " << slave_comm_size << " processors" << endl;
			} 
			else if (*command_buffer == 3) {
				moveShutter();
				cout << "moveing shutter finished from processor " << rank << " out of " << slave_comm_size << " processors" << endl;
			} 
			else if (*command_buffer == 4) {
				simulate();
				cout << "simulation finished from processor " << rank << " out of " << slave_comm_size << " processors" << endl;
			} 
			else if (*command_buffer == 5) {
				render();
				cout << "rendering finished from processor " << rank << " out of " << slave_comm_size << " processors" << endl;
			}
		}
	}

	if (rank == 0) {
		system("pause");
		MPI_Wait;
	}

	MPI_Finalize();
}
