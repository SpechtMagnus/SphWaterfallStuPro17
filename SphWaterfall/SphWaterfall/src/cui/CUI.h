#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "../data/NullableWrapper.h"
#include "CommandHandler.h"

class CUI {
	public:
		CUI(CommandHandler);

		void start();
		void printInputMessage();

	private:
		CommandHandler command_handler;
		CUICommand current_command;
		bool exit_programm;

		void startWithStream(std::istream&, bool);

		void parseCommand(std::string);
		void cleanAndExecuteCommand(bool);

		void trim(std::string& string);
		std::string trimQuotemarks(std::string string);

		void printCommand();
		void loadConfig();
		void showHelp();
		bool cleanLoadMesh();
		bool cleanLoadShutter();
		bool cleanMoveShutter();
		bool cleanAddSource();
		bool cleanAddSink();
		bool cleanSimulate();
};