#include"ActionHandler.h"


void ActionHandler::executeAction()
{
	switch (action)
	{
	case ActionHandler::Action::ZIP: {
		handler.createArchive(archivePath, arguments);
	}
								   break;
	case ActionHandler::Action::UNZIP: {
		if (arguments.size() == 1)
		{
			handler.extractArchive(arguments[0], archivePath);
		}
		else
		{
			std::filesystem::path dest = arguments.back();
			arguments.pop_back();
			for (std::filesystem::path path : arguments)
			{
				handler.unzipFile(path, archivePath, dest);
			}
		}
	}
									 break;
	case ActionHandler::Action::FILEINFO: {
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



void ActionHandler::parseCommand(std::string command)
{
	
	if (command == "zip")
		action = Action::ZIP;
	if (command == "unzip")
		action = Action::UNZIP;
	if (command == "info")
		action = Action::FILEINFO;
	if (command == "refresh")
		action = Action::REFRESH;
	if (command == "ec")
		action = Action::EC;
}

void ActionHandler::validateArguments(int argc, char** argv)
{
	if (argc < 3)
		throw std::invalid_argument("Provide enough arguments. Check Usage: ");


	archivePath = *(argv + 2);
	if (archivePath.extension() != ".lzw")
		throw std::invalid_argument("Please provide valid file for archive. Check Usage: ");

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
		if (argc < 4) throw std::invalid_argument("Provide enough arguments for unzip command. Check Usage:");
		
		if (argc > 4)
		{
			for (int i = 4; i < argc; i++)
			{
				arguments.push_back(std::filesystem::path(*(argv + i)));
			}
		}
		arguments.push_back(std::filesystem::path(*(argv + 3)));
	}
									 break;
	case ActionHandler::Action::FILEINFO: {
		if (argc >= 4) throw std::invalid_argument("Too much arguments for FILEINFO command. Check Usage:");
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

ActionHandler::Action ActionHandler::getAction() const
{
	return action;
}
