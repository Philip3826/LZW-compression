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
	ActionHandler(const std::string& command,char** argv);
	~ActionHandler();
	ActionHandler(const ActionHandler& other) = delete;
	ActionHandler& operator = (const ActionHandler& other) = delete;
	void executeAction();
	const Action getAction() const;
private:
	void validateArguments(char** argv);
	std::string rawCommand;
	Action action{ Action::ERROR };
	ArchiveHandler handler;
	std::vector <std::filesystem::path> arguments;
	
};