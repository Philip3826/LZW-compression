#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include "ArchiveHandler.h"

class ActionHandler {
public:
	ActionHandler(int argc, char** argv);
	~ActionHandler() = default;
	ActionHandler(const ActionHandler& other) = delete;
	ActionHandler& operator = (const ActionHandler& other) = delete;
	void executeAction();
	enum class Action
	{
		ZIP,
		UNZIP,
		INFO,
		REFRESH,
		EC,
		ERROR,
	};
private:
	std::filesystem::path archivePath{""};
	std::vector <std::filesystem::path> arguments;
	Action action{ Action::ERROR };
	ArchiveHandler handler;
	void validateArguments(int argc, char** argv);
	
};