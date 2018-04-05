#include "CUICommand.h"

CUICommand::CUICommand() :
	command(Command::NONE),
	input_line(""),
	command_name(""),
	parameter_list(std::vector<CUICommandParameter>()) {

}

CUICommand::CUICommand(std::string command_name, std::string input_line) :
	command(Command::NONE),
	input_line(input_line),
	command_name(command_name),
	parameter_list(std::vector<CUICommandParameter>()) {

}

CUICommand::CUICommand(std::string command_name, std::string input_line, Command command) :
	command(command),
	input_line(input_line),
	command_name(command_name),
	parameter_list(std::vector<CUICommandParameter>()) {

}

std::ostream& operator<<(std::ostream& out, const CUICommand& command) {
	out << "{" << "input_line:" << command.getInputLine() << " | command:" << command.getCommand()
		<< " | command_name:" << command.getCommandName() << " | parmeter_list:[";
	for (auto parameter : command.getParameterList()) {
		out << "(parameter_name:" << parameter.getParameterName() << " | parameter_value:" << parameter.getValue() <<  ")";
	}

	return out << "]}";
}

std::string CUICommand::getInputLine() const {
	return this->input_line;
}

CUICommand::Command CUICommand::getCommand() const{
	return this->command;
}

void CUICommand::setCommand(Command command) {
	this->command = command;
}

std::string CUICommand::getCommandName() const {
	return this->command_name;
}

std::vector<CUICommandParameter> CUICommand::getParameterList() const {
	return this->parameter_list;
}

CUICommandParameter CUICommand::getParameter(int index) const {
	return parameter_list.at(index);
}

void CUICommand::addParameter(CUICommandParameter parameter) {
	parameter_list.push_back(parameter);
}

bool CUICommand::removeParameter(CUICommandParameter parameter_to_remove) {
	for (int i = 0; i < parameter_list.size(); i++) {
		CUICommandParameter& parameter = parameter_list.at(i);
		if (parameter == parameter_to_remove) {
			parameter_list.erase(parameter_list.begin() + i);
			return true;
		}
	}
	return false;
}

bool CUICommand::removeParameter(std::string parameter_name) {
	for (int i = 0; i < parameter_list.size(); i++) {
		CUICommandParameter& parameter = parameter_list.at(i);
		if (parameter.getParameterName() == parameter_name) {
			parameter_list.erase(parameter_list.begin() + i);
			return true;
		}
	}
	return false;
}

bool CUICommand::hasParameter(std::string parameter_name) {
	for (int i = 0; i < parameter_list.size(); i++) {
		CUICommandParameter& parameter = parameter_list.at(i);
		if (parameter.getParameterName() == parameter_name) {
			return true;
		}
	}
	return false;
}
