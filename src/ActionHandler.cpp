#include"ActionHandler.h"

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
	switch (action)
	{
	case ActionHandler::Action::ZIP: {
		handler.createArchive(archivePath, arguments);
	}
								   break;
	case ActionHandler::Action::UNZIP: {

	}
									 break;
	case ActionHandler::Action::INFO: {
		handler.printInfo(archivePath);
	}
									break;
	case ActionHandler::Action::REFRESH: {
		handler.refreshFile(arguments[0], arguments[1], archivePath);
	}
									   break;
	case ActionHandler::Action::EC: {
		handler.checkForCorruption(archivePath);
	}
								  break;
	}
}



void ActionHandler::validateArguments(int argc, char** argv)
{
	if (argc < 3)
		throw std::invalid_argument("Provide enough arguments. Check Usage: ");

	archivePath = *(argv + 2);

	switch (action)
	{
	case ActionHandler::Action::ZIP: {
		if (argc < 4) throw std::invalid_argument("Provide enough arguments for zip command. Check Usage:");

		for (int i = 3; i < argc; i++)
		{
			arguments.push_back(std::filesystem::path(*(argv + i)));
		}

	}
								   break;
	case ActionHandler::Action::UNZIP: {
		if (argc >= 4)
		{
			for (int i = 3; i < argc; i++)
			{
				arguments.push_back(std::filesystem::path(*(argv + i)));
			}
		}
	}
									 break;
	case ActionHandler::Action::INFO: {
		if (argc >= 4) throw std::invalid_argument("Too much arguments for info command. Check Usage:");
	}
									break;
	case ActionHandler::Action::REFRESH: {
		if (argc != 5) throw std::invalid_argument("Need exactly 4 arguments for refresh command. Check Usage:");

		arguments.push_back(std::filesystem::path(*(argv + 3)));
		arguments.push_back(std::filesystem::path(*(argv + 4)));

	}
									   break;
	case ActionHandler::Action::ERROR: {
		throw std::invalid_argument("Invalid command. Check Usage:");
	}
									 break;
	default:
		break;
	}
}