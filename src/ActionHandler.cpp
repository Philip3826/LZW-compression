#include "ActionHandler.h"
#include "ActionHandler.h"

ActionHandler::ActionHandler(int argc, char** argv) 
{
	std::string command = argv[1];

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

	validateArguments(argc, argv);
}





void ActionHandler::executeAction()
{
	return;
}



void ActionHandler::validateArguments(int argc, char** argv)
{
	if (argc < 3)
		throw std::invalid_argument("Provide enough arguments. Check Usage: ");

	archivePath = argc[2];

	switch (action)
	{
	case ActionHandler::Action::ZIP: {
		if (argc <= 4) throw std::invalid_argument("Provide enough arguments for zip command. Check Usage:");

		for (int i = 2 ; i < argc ; i++)

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
