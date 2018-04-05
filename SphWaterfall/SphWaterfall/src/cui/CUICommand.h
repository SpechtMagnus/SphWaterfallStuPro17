#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "CUICommandParameter.h"

class CUICommand {
	public:
		enum Command
		{
			NONE = -1,
			EXIT = 0,
			LOAD_MESH,
			LOAD_SHUTTER,
			GENERATE_PARTICLES,
			MOVE_SHUTTER,
			SIMULATE,
			RENDER,
			ADD_SOURCE,
			ADD_SINK
		};

		CUICommand();
		CUICommand(std::string command_name, std::string input_line);
		CUICommand(std::string command_name, std::string input_line, Command command);

		friend std::ostream& operator<<(std::ostream& out, const CUICommand&);

		std::string getInputLine() const;
		Command getCommand() const;
		void setCommand(Command command);
		std::string getCommandName() const;
		std::vector<CUICommandParameter> getParameterList() const;
		CUICommandParameter getParameter(int index) const;
		void addParameter(CUICommandParameter parameter);
		bool removeParameter(CUICommandParameter parameter);
		bool removeParameter(std::string parameter_name);
		bool hasParameter(std::string parameter_name);
	private:
		std::string input_line;
		Command command;
		std::string command_name;
		std::vector<CUICommandParameter> parameter_list;
};
