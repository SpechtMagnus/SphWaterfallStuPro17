#include "CommandHandler.h"

CommandHandler::CommandHandler(int mpi_rank) : 
	mpi_rank(mpi_rank),
	simulation_time(DEFAULT_SIMULATION_TIME),
	sph_manager(SphManager(Vector3(DOMAIN_DIMENSION, DOMAIN_DIMENSION, DOMAIN_DIMENSION))){
}

void CommandHandler::start() {
	CUICommand cui_command;

	do {
		cui_command = recieveCommand();
		executeCommand(cui_command);
	} while (cui_command.getCommand() != CUICommand::EXIT);
}

void CommandHandler::handleCUICommand(CUICommand& cui_command) {
	sendCommand(cui_command);
	executeCommand(cui_command);
}

CUICommand CommandHandler::recieveCommand() {
	CUICommand::Command recieved_command;
	std::string recieved_input_line, recieved_command_name, recieved_parameter_name, recieved_parameter_value;
	int recieved_string_length;

	// command
	MPI_Bcast(&recieved_command, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// input line
	MPI_Bcast(&recieved_string_length, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (recieved_string_length > 0) {
		std::vector<char> input_line_buffer(recieved_string_length);
		MPI_Bcast(input_line_buffer.data(), recieved_string_length, MPI_CHAR, 0, MPI_COMM_WORLD);
		recieved_input_line = std::string(input_line_buffer.begin(), input_line_buffer.end());
	}
	else {
		recieved_input_line = "";
	}

	// command name
	MPI_Bcast(&recieved_string_length, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (recieved_string_length > 0) {
		std::vector<char> command_name_buffer(recieved_string_length);
		MPI_Bcast(command_name_buffer.data(), recieved_string_length, MPI_CHAR, 0, MPI_COMM_WORLD);
		recieved_command_name = std::string(command_name_buffer.begin(), command_name_buffer.end());
	}
	else {
		recieved_command_name = "";
	}

	// create return command to add parameters
	CUICommand cui_command = CUICommand(recieved_command_name, recieved_input_line, recieved_command);

	// parameter
	int number_of_parameters;
	MPI_Bcast(&number_of_parameters, 1, MPI_INT, 0, MPI_COMM_WORLD);
	for (int i = 0; i < number_of_parameters; i++) {
		// send parameter name
		MPI_Bcast(&recieved_string_length, 1, MPI_INT, 0, MPI_COMM_WORLD);
		if (recieved_string_length > 0) {
			std::vector<char> parameter_name_buffer(recieved_string_length);
			MPI_Bcast(parameter_name_buffer.data(), recieved_string_length, MPI_CHAR, 0, MPI_COMM_WORLD);
			recieved_parameter_name = std::string(parameter_name_buffer.begin(), parameter_name_buffer.end());
		}
		else {
			recieved_parameter_name = "";
		}

		// send parameter value
		MPI_Bcast(&recieved_string_length, 1, MPI_INT, 0, MPI_COMM_WORLD);
		if (recieved_string_length > 0) {
			std::vector<char> parameter_value_buffer(recieved_string_length);
			MPI_Bcast(parameter_value_buffer.data(), recieved_string_length, MPI_CHAR, 0, MPI_COMM_WORLD);
			recieved_parameter_value = std::string(parameter_value_buffer.begin(), parameter_value_buffer.end());
		}
		else {
			recieved_parameter_value = "";
		}
		cui_command.addParameter(CUICommandParameter(recieved_parameter_name, recieved_parameter_value));
	}

	return cui_command;
}

void CommandHandler::sendCommand(CUICommand& cui_command) {
	CUICommand::Command send_command = cui_command.getCommand();
	std::string send_input_line = cui_command.getInputLine();
	std::string send_command_name = cui_command.getCommandName();
	std::vector<CUICommandParameter> send_parameter_list = cui_command.getParameterList();
	std::string send_parameter_name, send_parameter_value;

	// send command
	MPI_Bcast(&send_command, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// send input line
	int input_line_length = send_input_line.size();
	MPI_Bcast(&input_line_length, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (input_line_length > 0) {
		std::vector<char> input_line_buffer(send_input_line.begin(), send_input_line.end());
		MPI_Bcast(input_line_buffer.data(), send_input_line.size(), MPI_CHAR, 0, MPI_COMM_WORLD);
	}

	// send command name
	int command_name_length = send_command_name.size();
	MPI_Bcast(&command_name_length, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (command_name_length > 0) {
		std::vector<char> command_name_buffer(send_command_name.begin(), send_command_name.end());
		MPI_Bcast(command_name_buffer.data(), send_command_name.size(), MPI_CHAR, 0, MPI_COMM_WORLD);
	}

	// send parameter
	int number_of_parameters = send_parameter_list.size();
	MPI_Bcast(&number_of_parameters, 1, MPI_INT, 0, MPI_COMM_WORLD);
	for (auto& parameter : send_parameter_list) {
		// send parameter name
		send_parameter_name = parameter.getParameterName();
		int parameter_name_length = send_parameter_name.size();
		MPI_Bcast(&parameter_name_length, 1, MPI_INT, 0, MPI_COMM_WORLD);
		if (parameter_name_length > 0) {
			std::vector<char> parameter_name_buffer(send_parameter_name.begin(), send_parameter_name.end());
			MPI_Bcast(parameter_name_buffer.data(), send_parameter_name.size(), MPI_CHAR, 0, MPI_COMM_WORLD);
		}

		// send parameter value
		send_parameter_value = parameter.getValue();
		int parameter_value_length = send_parameter_value.size();
		MPI_Bcast(&parameter_value_length, 1, MPI_INT, 0, MPI_COMM_WORLD);
		if (parameter_value_length > 0) {
			std::vector<char> parameter_value_buffer(send_parameter_value.begin(), send_parameter_value.end());
			MPI_Bcast(parameter_value_buffer.data(), send_parameter_value.size(), MPI_CHAR, 0, MPI_COMM_WORLD);
		}
	}
}

void CommandHandler::executeCommand(CUICommand& cui_command) {
	std::string file_path, time_for_move, source_position, sink_height;
	int simulation_timesteps;
	Vector3 camera_position = Vector3(0, 5, -5);
	unsigned int width = 800, height = 600;

	switch (cui_command.getCommand()) {
		case CUICommand::LOAD_MESH:
			if (mpi_rank == 0) {
				file_path = cui_command.getParameter(0).getValue();
				loaded_mesh = loadMesh(file_path);

				// console feedback
				cout << "Mesh loaded." << endl;
				cout << "Vertices: " << loaded_mesh.getVertexCount() << " Faces: " << loaded_mesh.getFaceCount() << endl;
			}
			MPI_Barrier(MPI_COMM_WORLD);
			break;
		case CUICommand::LOAD_SHUTTER:
			if (mpi_rank == 0) {
				file_path = cui_command.getParameter(0).getValue();
				loaded_shutter = loadMesh(file_path);

				// console feedback
				cout << "Mesh loaded." << endl;
				cout << "Vertices: " << loaded_shutter.getVertexCount() << " Faces: " << loaded_shutter.getFaceCount() << endl;
			}
			MPI_Barrier(MPI_COMM_WORLD);
			break;
		case CUICommand::GENERATE_PARTICLES:
			generateParticles(loaded_mesh, SphParticle::ParticleType::STATIC);
			generateParticles(loaded_shutter, SphParticle::ParticleType::SHUTTER);

			// console feedback
			MPI_Barrier(MPI_COMM_WORLD);
			if (mpi_rank == 0) {
				cout << "Static particles generated." << endl;
			}
			break;
		case CUICommand::MOVE_SHUTTER:
			time_for_move = cui_command.getParameter(0).getValue();
			moveShutter(time_for_move);
			MPI_Barrier(MPI_COMM_WORLD);

			// console feedback
			if (mpi_rank == 0) {
				cout << "Stutter move set." << endl;
			}
			break;
		case CUICommand::SIMULATE:
			if (cui_command.hasParameter("-t")) {
				simulation_timesteps = round(parseToDouble(cui_command.getParameter(0).getValue()) / TIMESTEP_DURATION);
			}
			else {
				simulation_timesteps = round(simulation_time / TIMESTEP_DURATION);
			}
			
			if (mpi_rank != 0) {
				simulate(simulation_timesteps);
			}
			else {
				createExport(simulation_timesteps);
			}
			MPI_Barrier(MPI_COMM_WORLD);

			// console feedback
			if (mpi_rank == 0) {
				cout << "Simulation finished." << endl;
			}
			break;
		case CUICommand::RENDER:
			if (cui_command.hasParameter("-v")) {
				camera_position = parseToVector3(cui_command.getParameter(cui_command.getParameterIndex("-v")).getValue());
			}

			if (cui_command.hasParameter("-w") && cui_command.hasParameter("-h")) {
				width = parseToUInteger(cui_command.getParameter(cui_command.getParameterIndex("-w")).getValue());
				height = parseToUInteger(cui_command.getParameter(cui_command.getParameterIndex("-h")).getValue());
			}

			render(loaded_mesh, loaded_shutter, 0, camera_position, width, height);
			MPI_Barrier(MPI_COMM_WORLD);

			// console feedback
			if (mpi_rank == 0) {
				cout << "Rendering finished." << endl;
			}
			break;
		case CUICommand::ADD_SOURCE:
			if (mpi_rank != 0) {
				source_position = cui_command.getParameter(0).getValue();
				addSource(source_position);
			}
			MPI_Barrier(MPI_COMM_WORLD);

			// console feedback
			if (mpi_rank == 0) {
				cout << "Added source." << endl;
			}
			break;
		case CUICommand::ADD_SINK:
			if (mpi_rank != 0) {
				sink_height = cui_command.getParameter(0).getValue();
				addSink(sink_height);
			}
			MPI_Barrier(MPI_COMM_WORLD);

			// console feedback
			if (mpi_rank == 0) {
				cout << "Added sink." << endl;
			}
			break;
		default:
			break;
	}
}


int CommandHandler::parseToInteger(std::string input) {
	std::istringstream input_parse_stream(input);
	int output;
	input_parse_stream >> output;
	return output;
}

unsigned int CommandHandler::parseToUInteger(std::string input) {
	int result = this->parseToInteger(input);
	return (result > 0) ? result : 0;
}

double CommandHandler::parseToDouble(std::string input) {
	std::istringstream input_parse_stream(input);
	double output;
	input_parse_stream >> output;
	return output;
}

Vector3 CommandHandler::parseToVector3(std::string input) {
	std::istringstream input_parse_stream(input);
	double x, y, z;
	input_parse_stream >> x;
	input_parse_stream >> y;
	input_parse_stream >> z;
	return Vector3(x, y, z);
}

void CommandHandler::printInputMessage() {
	std::cout << std::endl << "Please enter a command or enter 'help' to show a list of all commands" << std::endl;
}

Terrain CommandHandler::loadMesh(std::string file_path) {
	std::cout << "Loading Mesh: \"" << file_path << "\"" << std::endl;
	return TerrainParser::loadFromFile(file_path);
}

void CommandHandler::generateParticles(Terrain& loaded_mesh, SphParticle::ParticleType particle_type) {

	if (mpi_rank == 0) {
		StaticParticleGenerator::sendAndGenerate(loaded_mesh, particle_type);
	}
	else {
		StaticParticleGenerator::receiveAndGenerate(sph_manager, particle_type);
	}
}

void CommandHandler::createExport(int simulation_timesteps) {
	int current_timestep = 1;
	unordered_map<int, vector<SphParticle>> export_map;

	int slave_comm_size;
	MPI_Comm_size(MPI_COMM_WORLD, &slave_comm_size);
	slave_comm_size--;

	while (current_timestep <= simulation_timesteps) {
		std::vector<SphParticle> all_particles_of_timestep;

		std::vector<int> number_of_incoming_particles = std::vector<int>(slave_comm_size);
		std::unordered_map<int, std::vector<SphParticle>> incoming_particles;

		MPI_Barrier(MPI_COMM_WORLD);

		for (int i = 0; i < slave_comm_size; i++) {
			MPI_Recv(&number_of_incoming_particles[i], 1, MPI_INT, i + 1, EXPORT_PARTICLES_NUMBER_TAG, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
			incoming_particles[i] = std::vector<SphParticle>(number_of_incoming_particles.at(i));
		}

		for (int i = 0; i < slave_comm_size; i++) {
			if (number_of_incoming_particles.at(i) != 0) {
				MPI_Recv(incoming_particles[i].data(), number_of_incoming_particles.at(i) * sizeof(SphParticle), MPI_BYTE, i + 1, EXPORT_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				all_particles_of_timestep.insert(all_particles_of_timestep.end(), incoming_particles.at(i).begin(), incoming_particles.at(i).end());
			}
		}

		export_map[current_timestep] = all_particles_of_timestep;
		ParticleIO::exportParticlesToVTK(all_particles_of_timestep, "vtk/particles", current_timestep, number_of_incoming_particles);

		current_timestep++;
	}
	ParticleIO::exportParticles(export_map, "sph.ptcl");

	std::cout << "Done exporting" << std::endl;
}

void CommandHandler::moveShutter(std::string shutter_move_param) {
	int worldSize;
	MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
	int shutter_move_frame;
	
	if (mpi_rank == 0) {
		shutter_move_frame = round(parseToDouble(shutter_move_param) / TIMESTEP_DURATION);

		for (int i = 1; i < worldSize; i++) {
			MPI_Send(&shutter_move_frame, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}
	else {
		MPI_Recv(&shutter_move_frame, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	sph_manager.setShutterTimestep(shutter_move_frame);
	VisualizationManager::setSwitchFrame(shutter_move_frame);
	std::cout << "Shutter opening at frame: " << shutter_move_frame << std::endl;
}

void CommandHandler::simulate(int simulation_timesteps) {
	if (mpi_rank == -1) {
		std::vector<SphParticle> particles;

		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				for (int k = 0; k < 20; k++) {
					particles.push_back(SphParticle(Vector3(5.0 + i, 10.0 + j, 5.0 + k)));
				}
			}
		}

		sph_manager.add_particles(particles);
	}
	sph_manager.simulate(simulation_timesteps);
}

void CommandHandler::render(Terrain loaded_mesh, Terrain loaded_shutter, int shutter_time, Vector3 cameraPosition, unsigned int width, unsigned int height) {
	int worldSize;
	MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
	
	VisualizationManager::importTerrain(loaded_mesh, false);
	VisualizationManager::importTerrain(loaded_shutter, true);

	if (mpi_rank == 0) {

		for (int i = 1; i < worldSize; i++) {
			MPI_Send(&width, 1, MPI_UNSIGNED, i, 0, MPI_COMM_WORLD);
			MPI_Send(&height, 1, MPI_UNSIGNED, i, 0, MPI_COMM_WORLD);
		}

		cout << "Rendering in progress with (" << width << " * " << height << ") ..." << endl;
	}
	else {
		MPI_Recv(&width, 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&height, 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	VisualizationManager::init(cameraPosition, width, height, mpi_rank);
	//VisualizationManager::renderFrames("sph.ptcl");
	VisualizationManager::renderFramesDistributed("sph.ptcl", mpi_rank);

	MPI_Barrier(MPI_COMM_WORLD);

	if (mpi_rank == 0) {
		cout << "Rendering complete" << endl;
	}
}

void CommandHandler::addSource(std::string source_position_string) {
	std::istringstream source_position_stream(source_position_string);
	double x, y, z;
	source_position_stream >> x;
	source_position_stream >> y;
	source_position_stream >> z;
	Vector3 source_position = Vector3(x, y, z);
	int proccess_id = SimulationUtilities::computeProcessID(source_position, sph_manager.getDomainDimensions());
	if (proccess_id + 1 == mpi_rank) {
		sph_manager.addSource(source_position);
		std::cout << "Source added at " << source_position << std::endl;
	}
}

void CommandHandler::addSink(std::string sink_height_string) {
	std::istringstream sink_height_stream(sink_height_string);
	double sink_height;
	sink_height_stream >> sink_height;
	sph_manager.setSink(sink_height);
	std::cout << "Sink added at y=" << sink_height << std::endl;
}
