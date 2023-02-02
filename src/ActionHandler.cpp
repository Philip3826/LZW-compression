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

void ActionHandler::validateArguments(std::vector<std::string> argv)
{
	if (argv.size() == 0)
		throw std::invalid_argument("Provide enough arguments. Check Usage: ");


	archivePath = argv[0];
	if (archivePath.extension() != ".lzw")
		throw std::invalid_argument("Please provide valid file for archive. Check Usage: ");

	switch (action)
	{
	case ActionHandler::Action::ZIP: {
		if (argv.size() < 2) throw std::invalid_argument("Provide enough arguments for zip command. Check Usage:");

		for (int i = 1; i < argv.size(); i++)
		{
			arguments.push_back(std::filesystem::path(argv[i]));
		}

	}break;

	case ActionHandler::Action::UNZIP: {
		if (argv.size() < 2) throw std::invalid_argument("Provide enough arguments for unzip command. Check Usage:");
		
		if (argv.size() > 2)
		{
			for (int i = 2; i < argv.size(); i++)
			{
				arguments.push_back(std::filesystem::path(argv[i]));
			}
		}
		arguments.push_back(std::filesystem::path(argv[1]));
	}
									 break;
	case ActionHandler::Action::FILEINFO: {
		if (argv.size() >= 2) throw std::invalid_argument("Too much arguments for FILEINFO command. Check Usage:");
	}
									break;
	case ActionHandler::Action::REFRESH: {
		if (argv.size() != 3) throw std::invalid_argument("Need exactly 4 arguments for refresh command. Check Usage:");

		arguments.push_back(std::filesystem::path(argv[1]));
		arguments.push_back(std::filesystem::path(argv[2]));

	}
									   break;
	case ActionHandler::Action::EC: {
		if(argv.size() >= 2) throw std::invalid_argument("Too much arguments for EC command. Check Usage:");
	}	break;
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
