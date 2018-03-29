#include <thread>

#include "mpi.h"

#include "cui/CUI.h"
#include "simulation/SimulationUtilities.h"
#include "data/SphParticle.h"
#include "particleGen/StaticParticleGenerator.h"
#include "visualization/VisualizationManager.h"
#include "data/ParticleIO.h"

CUI::AsyncCommand acmd;

void loadMesh(int rank, std::string fileName, Terrain& loadedMesh) {
	std::cout << "Loading Mesh: \"" << fileName << "\"" << std::endl;
	loadedMesh = TerrainParser::loadFromFile(fileName);
	cout << "Vertices: " << loadedMesh.getVertexCount() << " Faces: " << loadedMesh.getFaceCount() << endl;
	VisualizationManager::importTerrain(loadedMesh, true);
	VisualizationManager::importTerrain(loadedMesh, false);
}

void generateParticles(int rank, SphManager& sphManager, Terrain& loadedMesh, SphParticle::ParticleType pType) {
	StaticParticleGenerator gen;

	if (rank == 0)
		gen.sendAndGenerate(loadedMesh, pType);
	else
		gen.receiveAndGenerate(sphManager, pType);
}

void createExport(int rank, SphManager& sph_manager) {
		int current_timestep = 1;
		int current_frame = 1;

		unordered_map<int, vector<SphParticle>> export_map;

		while (current_timestep <= TIMESTEPS) {
			std::unordered_map<int, std::vector<SphParticle>> allParticles;

			std::vector<SphParticle> allParticlesOfTimestep;

			// receive until there is nothing left
			int flag = 0;
			MPI_Status status;

			// custom "barrier" to fix MPI_Send in SphManager from deadlocking the programm
			while (!flag) {
				MPI_Iprobe(MPI_ANY_SOURCE, EXPORT_TAG, MPI_COMM_WORLD, &flag, &status);
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}

			int count = 0;
			int source;

			while (flag) {
				source = status.MPI_SOURCE;
				MPI_Get_count(&status, MPI_BYTE, &count);
				std::vector<SphParticle> incomingParticles = std::vector<SphParticle>(count / sizeof(SphParticle));

				MPI_Recv(incomingParticles.data(), count, MPI_BYTE, source, EXPORT_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				allParticlesOfTimestep.insert(allParticlesOfTimestep.end(), incomingParticles.begin(), incomingParticles.end());

				// for (auto particle : allParticlesOfTimestep) { std::cout << current_timestep << " received in export: " << particle << std::endl; } // debug

				// next message
				MPI_Iprobe(MPI_ANY_SOURCE, EXPORT_TAG, MPI_COMM_WORLD, &flag, &status);
			}
			
			export_map[current_frame] = allParticlesOfTimestep;
			ParticleIO::exportParticlesToVTK(allParticlesOfTimestep, "vtk/particles", current_timestep);

			current_frame++;
			current_timestep++;
		}
		ParticleIO::exportParticles(export_map, "test.test");
		
		std::cout << "Done exporting" << std::endl;
}

void moveShutter(int rank, SphManager& sphManager, std::string shutterMoveParam)
{
	std::istringstream shutterMoveValues(shutterMoveParam);
	int shutterMoveFrame;
	shutterMoveValues >> shutterMoveFrame;
	std::cout << "Shutter opening at frame: " << shutterMoveFrame << std::endl;
}

void simulate(int rank, SphManager& sph_manager) {
	cout << "command is simulate" << endl;

	if (rank == 1) {
		std::vector<SphParticle> particles;
		
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				for (int k = 0; k < 20; k++) {
					//SphParticle particle = SphParticle(Vector3(1000.0 + (i/10.0), 1000.0 + (j/10.0), 1000.0 + (k/10.0)));
					SphParticle particle = SphParticle(Vector3(3.0 + i, 3.0 + j, 3.0 + k));
					particles.push_back(particle);
					//cout << particle.position << endl;
				}
			}
		}
		

		//particles.push_back(SphParticle(Vector3(10.0, 10.0, 10.0)));
		sph_manager.add_particles(particles);
	}
	sph_manager.simulate();
}


void render(int rank) {

	if (rank == 0) {
		cout << "Rendering in progress..." << endl;
	}
	VisualizationManager::init(Vector3(10, 5, -20), 200, 200);
	//VisualizationManager::renderFrames("test.test");
	VisualizationManager::renderFramesDistributed("test.test", rank);

	MPI_Barrier(MPI_COMM_WORLD);

	if (rank == 0) {
		cout << "Rendering complete" << endl; 
	}
}

void addSource(int rank, SphManager& sphManager, std::string sourcePosParam)
{
	std::istringstream sourcePosValues(sourcePosParam);
	double x, y, z;
	sourcePosValues >> x;
	sourcePosValues >> y;
	sourcePosValues >> z;
	Vector3 sourcePos = Vector3(x, y, z);
	std::cout << "New source: " << sourcePos << std::endl;
}

void addSink(int rank, SphManager& sphManager, std::string sinkHeightParam)
{
	std::istringstream sinkHeightValues(sinkHeightParam);
	double sinkHeight;
	sinkHeightValues >> sinkHeight;
	std::cout << "New sink height: " << sinkHeight << std::endl;
}

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// generate slave_comm and slave_comm_size for simulation
	int color = 1337;
	if (rank == 0) {
		color = MPI_UNDEFINED;
	}
	MPI_Comm_split(MPI_COMM_WORLD, color, 0, &slave_comm);
	if (rank != 0) {
		MPI_Comm_size(slave_comm, &slave_comm_size);
	}

	std::thread cuiThread;
	int cmd = CUI::ConsoleCommand::NONE;
	std::string cmdParam;

	SphManager sphManager = SphManager(Vector3(DOMAIN_DIMENSION, DOMAIN_DIMENSION, DOMAIN_DIMENSION), TIMESTEPS, 0.03);
	Terrain loadedMesh;
	Terrain loadedShutter;

	if (rank == 0) {
		cuiThread = std::thread(CUI::startCUI);
	}

	while (cmd != CUI::ConsoleCommand::EXIT) {
		if (rank == 0) {
			cmd = CUI::acmd.readCmd(cmdParam);
		}
		MPI_Bcast(&cmd, 1, MPI_INT, 0, MPI_COMM_WORLD);

		//Execute console input
		switch (cmd)
		{
		case CUI::ConsoleCommand::LOAD_MESH:
			loadMesh(rank, cmdParam, loadedMesh);
			MPI_Barrier(MPI_COMM_WORLD);
			if (rank == 0) {
				cout << "Mesh loaded." << endl;
				acmd.printInputMessage();
			}
			break;
		case CUI::ConsoleCommand::LOAD_SHUTTER:
			loadMesh(rank, cmdParam, loadedShutter);
			MPI_Barrier(MPI_COMM_WORLD);
			if (rank == 0) {
				cout << "Mesh loaded." << endl;
				acmd.printInputMessage();
			}
			break;
		case CUI::ConsoleCommand::GENERATE_PARTICLES:
			generateParticles(rank, sphManager, loadedMesh, SphParticle::ParticleType::STATIC);
			generateParticles(rank, sphManager, loadedShutter, SphParticle::ParticleType::SHUTTER);
			MPI_Barrier(MPI_COMM_WORLD);
			if (rank == 0) {
				cout << "Static particles generated." << endl;
				acmd.printInputMessage();
			}
			break;
		case CUI::ConsoleCommand::MOVE_SHUTTER:
			moveShutter(rank, sphManager, cmdParam);
			MPI_Barrier(MPI_COMM_WORLD);
			if (rank == 0) {
				cout << "Stutter move set." << endl;
				acmd.printInputMessage();
			}
			break;
		case CUI::ConsoleCommand::SIMULATE:
			if (rank != 0) {
				simulate(rank, sphManager);
			}
			else{
				createExport(rank, sphManager);
			}
			MPI_Barrier(MPI_COMM_WORLD);
			if (rank == 0) {
				cout << "Simulation finished." << endl;
				acmd.printInputMessage();
			}
			break;
		case CUI::ConsoleCommand::RENDER:
			render(rank);
			MPI_Barrier(MPI_COMM_WORLD);
			if (rank == 0) {
				cout << "Rendering finished." << endl;
				acmd.printInputMessage();
			}
		case CUI::ConsoleCommand::ADD_SOURCE:
			addSource(rank, sphManager, cmdParam);
			MPI_Barrier(MPI_COMM_WORLD);
			if (rank == 0) {
				cout << "Added source." << endl;
				acmd.printInputMessage();
			}
			break;
		case CUI::ConsoleCommand::ADD_SINK:
			addSink(rank, sphManager, cmdParam);
			MPI_Barrier(MPI_COMM_WORLD);
			if (rank == 0) {
				cout << "Added sink." << endl;
				acmd.printInputMessage();
			}
			break;
		default:
			break;
		}
	}

	if (rank == 0) {
		cuiThread.join();
		MPI_Wait;
	}

	MPI_Finalize();

	return 0;
}
