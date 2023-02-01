#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include "ArchiveHandler.h"

class ActionHandler {
public:
	ActionHandler()  = default;
	~ActionHandler() = default;
	ActionHandler(const ActionHandler& other) = delete;
	ActionHandler& operator = (const ActionHandler& other) = delete;
	void executeAction();
	enum class Action
	{
		ZIP,
		UNZIP,
		FILEINFO,
		REFRESH,
		EC,
		ERROR,
	};
	void parseCommand(std::string command);
	void validateArguments(int argc, char** argv);
	Action getAction() const;
private:
	std::filesystem::path archivePath{""};
	std::vector <std::filesystem::path> arguments;
	Action action{ Action::ERROR };
	ArchiveHandler handler;
	
};