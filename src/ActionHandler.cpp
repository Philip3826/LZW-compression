#include "ActionHandler.h"

ActionHandler::ActionHandler(const std::string& command, char** argv) : rawCommand(command)
{
	if (command == "zip")
		action = Action::ZIP;
	if (command == "unzip")
		action = Action::UNZIP;
	if (command == "info")
		action = Action::INFO;
	if (command == "refresh")
		action = Action::REFRESH;
	if (command == "ec")
		action = Action::EC;
}

ActionHandler::~ActionHandler()
{
	
}

void ActionHandler::executeAction()
{

}

const ActionHandler::getAction() const
{
	return Action;
}

void ActionHandler::validateArguments(char** argv)
{
	switch (action)
	{
	case ActionHandler::Action::ZIP: {

	}
		break;
	case ActionHandler::Action::UNZIP:
		break;
	case ActionHandler::Action::INFO:
		break;
	case ActionHandler::Action::REFRESH:
		break;
	case ActionHandler::Action::EC:
		break;
	case ActionHandler::Action::ERROR:
		break;
	default:
		break;
	}
}
