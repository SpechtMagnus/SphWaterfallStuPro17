#pragma once
#include "CUI.h"
#include "../visualization/VisualizationManager.h"

CUI::CUI() {

}

void CUI::trim(std::string &str) {
	int pos1 = str.find_first_not_of(" ");
	int pos2 = str.find_last_not_of(" ");
	str = str.substr(pos1, pos2 - pos1 + 1);
}

bool CUI::readNextToken(std::queue<std::string> &tokens, std::string &nextToken)
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

bool CUI::readNextCombinedToken(std::queue<std::string> &tokens, std::string &nextToken) {
	std::string tempToken = "", resultToken = "";

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

void CUI::loadMesh(std::queue<std::string> &tokens)
{
	std::string paramName, fileName;

	//Read fileName Parameter
	if (readNextToken(tokens, paramName) && paramName == "-p") {
		fileName = "";
		readNextCombinedToken(tokens, fileName);
		std::cout << fileName << std::endl;

		Terrain loadedFile = TerrainParser::loadFromFile(fileName);
		std::cout << "Vertices: " << loadedFile.getVertexCount() << " Faces: " << loadedFile.getFaceCount() << std::endl;
	}
	else
	{
		std::cout << "Missing path parameter '-p'" << std::endl;
	}
}

void CUI::simulate(std::queue<std::string> &tokens)
{
	// TODO add correct dimensions
	SphManager sph_manager = SphManager::SphManager(Vector3(10, 10, 10));
}

void render()
{
	init(Vector3(0, 0, -30), Vector3(0, 0, 1), 1920, 1080);

	addCamera(Vector3(-30, 0, -30), normalizeVector(Vector3(1, 0, 1)), 1920, 1080);

	std::vector<SphParticle> particles;
	SphParticle particle = SphParticle(Vector3(0, 0, 0), Vector3(0, 0, 0));
	SphParticle particle2 = SphParticle(Vector3(0, 5, 0), Vector3(0, 0, 0));
	SphParticle particle3 = SphParticle(Vector3(5, 5, 0), Vector3(0, 0, 0));
	SphParticle particle4 = SphParticle(Vector3(-5, -5, -5), Vector3(0, 0, 0));

	particles.emplace_back(particle);
	particles.emplace_back(particle2);
	particles.emplace_back(particle3);
	particles.emplace_back(particle4);

	std::cout << "Rendering, please wait..." << std::endl;

	debugRenderFrame(particles);

	std::cout << "Done!" << std::endl;
}

void CUI::showHelp()
{
	std::cout << "loadMesh -p" << std::endl;
	std::cout << "particleGen [-w] [-f] [-e]" << std::endl;
	std::cout << "moveShutter -t (-u/-d) [-l]" << std::endl;
	std::cout << "simulate -s -e -r -g -m -t" << std::endl;
	std::cout << "render" << std::endl;
	std::cout << "help" << std::endl;
	std::cout << "exit" << std::endl;
}

/* -_-_-_Comands End_-_-_- */

void CUI::readCommands(int* buffer)
{
	while (true)
	{
		//Read command
		std::cout << std::endl << "Please enter a command or enter 'help' to show a list of all commands" << std::endl;
		std::getline(std::cin, inputLine);
		trim(inputLine);

		//Tokenize command
		std::istringstream tokenStream(inputLine);
		tokens = std::queue<std::string>();
		while (tokenStream >> command)
			tokens.push(command);

		//Execute command
		if (!tokens.empty())
		{
			command = tokens.front();
			tokens.pop();

			if (command == "loadMesh") {
				buffer[0] = 1;
				loadMesh(tokens);
			}
			else if (command == "particleGen") {
				buffer[0] = 2;
			}
			else if (command == "moveShutter") {
				buffer[0] = 3;
			}
			else if (command == "simulate") {
				buffer[0] = 4;
				//simulate(tokens);
			}
			else if (command == "render")
			{
				buffer[0] = 5;
				render();
			}
			else if (command == "help" || command == "?") {
				showHelp();
			}
			else if (command == "exit") {
				break;
			}
			else {
				std::cout << "Unknown command. Enter 'help' to view a list of all available commands." << std::endl;
			}
		}
	}
}