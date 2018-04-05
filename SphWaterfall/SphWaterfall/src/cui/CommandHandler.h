#pragma once
#include <string>
#include <vector>

#include "mpi.h"

#include "CUICommand.h"
#include "../simulation/SimulationUtilities.h"
#include "../data/SphParticle.h"
#include "../particleGen/StaticParticleGenerator.h"
#include "../geometry/TerrainParser.h"
#include "../visualization/VisualizationManager.h"
#include "../data/ParticleIO.h"

class CommandHandler {
	public:
		CommandHandler(int mpi_rank);

		void start();
		void handleCUICommand(CUICommand&);

		void printInputMessage();
	private:
		const int mpi_rank;

		SphManager sph_manager;
		Terrain loaded_mesh, loaded_shutter;
		int simulation_time;

		CUICommand recieveCommand();
		void sendCommand(CUICommand&);
		void executeCommand(CUICommand&);

		int parseToInteger(std::string);
		double parseToDouble(std::string);
		Vector3 parseToVector3(std::string);

		Terrain loadMesh(std::string);
		void generateParticles(Terrain&, SphParticle::ParticleType);
		void createExport(int simulation_timesteps);
		void moveShutter(std::string);
		void simulate(int simulation_timesteps);
		void render(Terrain, Terrain, int);
		void addSource(std::string);
		void addSink(std::string);
};
