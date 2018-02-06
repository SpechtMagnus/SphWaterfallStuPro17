#pragma once
#include "CUI.h"
#include "../visualization/VisualizationManager.h"

namespace CUI {

	void trim(std::string &str) {
		int pos1 = str.find_first_not_of(" ");
		int pos2 = str.find_last_not_of(" ");
		str = str.substr(pos1, pos2 - pos1 + 1);
	}

	bool readNextToken(std::queue<std::string> &tokens, std::string &nextToken)
	{
		if (tokens.empty()) {
			return false;
		}
		else {
			nextToken = tokens.front();
			tokens.pop();
			return true;
		}
	}

	bool readNextCombinedToken(std::queue<std::string> &tokens, std::string &nextToken) {
		string tempToken = "", resultToken = "";

		//Read first token
		if (!readNextToken(tokens, tempToken))
			return false;
		if (tempToken.find_first_of('\"') != 0) {
			//No combined token
			nextToken = tempToken;
			return true;
		}
		if (tempToken.find_last_of('\"') == tempToken.length() - 1) {
			//Combined token without spaces
			nextToken = tempToken.substr(1, tempToken.length() - 2);
			return true;
		}

		//Read following tokens
		do {
			resultToken += tempToken + " ";
			if (!readNextToken(tokens, tempToken)) {
				// No second quotation mark
				nextToken = resultToken.substr(1);
				return true;
			}
		} while (tempToken.find_last_of('\"') != tempToken.length() - 1);

		//Return combined token
		resultToken += tempToken;
		nextToken = resultToken.substr(1, resultToken.length() - 2);
		return true;
	}


	/* -_-_-_Comands Begin_-_-_- */

	void loadMesh(std::queue<std::string> &tokens)
	{
		string paramName, fileName;

		//Read fileName Parameter
		if (readNextToken(tokens, paramName) && paramName == "-p") {
			fileName = "";
			readNextCombinedToken(tokens, fileName);
			cout << fileName << endl;

			Terrain loadedFile = TerrainParser::loadFromFile(fileName);
			cout << "Vertices: " << loadedFile.getVertexCount() << " Faces: " << loadedFile.getFaceCount() << endl;
		}
		else
		{
			cout << "Missing path parameter '-p'" << endl;
		}
	}

	void simulate(std::queue<std::string> &tokens)
	{
		// TODO add correct dimensions
		// SphManager sph_manager = SphManager::SphManager(Vector3(10, 10, 10), 10, 1);
	}

	void render()
	{
		init(Vector3(0, 0, -30), Vector3(0, 0, 1), 1920, 1080);

		addCamera(Vector3(-30, 0, -30), normalizeVector(Vector3(1, 0, 1)), 1920, 1080);

		vector<FluidParticle> particles;
		FluidParticle particle = FluidParticle(Vector3(0, 0, 0), Vector3(0, 0, 0));
		FluidParticle particle2 = FluidParticle(Vector3(0, 5, 0), Vector3(0, 0, 0));
		FluidParticle particle3 = FluidParticle(Vector3(5, 5, 0), Vector3(0, 0, 0));
		FluidParticle particle4 = FluidParticle(Vector3(-5, -5, -5), Vector3(0, 0, 0));

		particles.emplace_back(particle);
		particles.emplace_back(particle2);
		particles.emplace_back(particle3);
		particles.emplace_back(particle4);

		cout << "Rendering, please wait..." << endl;

		debugRenderFrame(particles);

		cout << "Done!" << endl;
	}

	void showHelp()
	{
		cout << "loadMesh -p" << endl
			<< "particleGen [-w] [-f] [-e]" << endl
			<< "moveShutter -t (-u/-d) [-l]" << endl
			<< "simulate -s -e -r -g -m -t" << endl
			<< "render" << endl
			<< "help" << endl
			<< "exit" << endl;
	}

	/* -_-_-_Comands End_-_-_- */


	void readCommand(int* command_buffer)
	{
		string inputLine, command;
		queue<string> tokens;

		//Read command
		cout << endl << "Please enter a command or enter 'help' to show a list of all commands" << endl;
		getline(cin, inputLine);
		trim(inputLine);

		//Tokenize command
		istringstream tokenStream(inputLine);
		tokens = queue<string>();
		while (tokenStream >> command)
			tokens.push(command);

		//Execute command
		if (!tokens.empty())
		{
			command = tokens.front();
			tokens.pop();

			if (command == "loadMesh") {
				command_buffer[0] = 1;
				loadMesh(tokens);
			}
			else if (command == "particleGen") {
				command_buffer[0] = 2;
			}
			else if (command == "moveShutter") {
				command_buffer[0] = 3;
			}
			else if (command == "simulate") {
				command_buffer[0] = 4;
				//simulate(tokens);
			}
			else if (command == "render")
			{
				command_buffer[0] = 5;
				render();
			}
			else if (command == "help" || command == "?") {
				showHelp();
			}
			else if (command == "exit") {
				command_buffer[0] = 0;
			}
			else {
				cout << "Unknown command. Enter 'help' to view a list of all available commands." << endl;
			}
		}
	}
}
