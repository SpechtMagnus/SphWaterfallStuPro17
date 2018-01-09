#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>

void trim(std::string &str) {
	int pos1 = str.find_first_not_of(" ");
	int pos2 = str.find_last_not_of(" ");
	str = str.substr(pos1, pos2 - pos1 + 1);
}

bool readParameter(char paramName, std::istringstream &parameters)
{
	std::string nameToken, valueToken;
	return (parameters >> nameToken && nameToken == "-" + paramName);
}

std::string readNextToken(std::queue<std::string> &tokens)
{
	if (tokens.empty()) {
		return "";
	} else {
		auto result = tokens.front();
		tokens.pop();
		return result;
	}
}

/* -_-_-_Comands Begin_-_-_- */

void loadMesh(std::queue<std::string> &tokens)
{
	std::string paramName, fileName;

	//Read fileName Parameter
	paramName = readNextToken(tokens);
	if (paramName != "-p") {
		std::cout << "Missing path parameter '-p'" << std::endl;
	} else {
		fileName = readNextToken(tokens);
		std::cout << fileName << std::endl;
	}
}

void showHelp()
{
	std::cout << "loadMesh -p" << std::endl;
	std::cout << "particleGen [-w] [-f] [-e]" << std::endl;
	std::cout << "moveShutter -t (-u/-d) [-l]" << std::endl;
	std::cout << "simulate -s -e -r -g -m -t" << std::endl;
	std::cout << "help" << std::endl;
	std::cout << "exit" << std::endl;
}

/* -_-_-_Comands End_-_-_- */

void readCommands()
{
	std::string inputLine, command;
	std::queue<std::string> tokens;

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

			if (command == "loadMesh")
				loadMesh(tokens);
			else if (command == "help" || command == "?")
				showHelp();
			else if (command == "exit")
				break;
			else
				std::cout << "Unknown command. Enter 'hlep' to view a list of all available commands." << std::endl;
		}
	}
}

int main()
{
	readCommands();
}