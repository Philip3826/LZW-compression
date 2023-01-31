#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include "ArchiveHandler.h"
class ActionHandler {

public:
	enum class Action
	{
		ZIP,
		UNZIP,
		INFO,
		REFRESH,
		EC,
		ERROR,
	};
	ActionHandler(int argc, char** argv);
	~ActionHandler() = default;
	ActionHandler(const ActionHandler& other) = delete;
	ActionHandler& operator = (const ActionHandler& other) = delete;
	void executeAction();
private:
	Action action{ Action::ERROR };
	std::filesystem::path archivePath{""};
	ArchiveHandler handler;
	std::vector <std::filesystem::path> arguments;
	void validateArguments(int argc, char** argv);
	
};