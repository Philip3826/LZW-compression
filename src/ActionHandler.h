#pragma once
#include <string>
#include <vector>
#include <filesystem>

class ActionHandler {

private:
	std::string rawCommand;
	Action action;
	std::vector <std::filesystem::path> arguments;
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
	ActionHandler(const std::string command) : rawCommand(command);
	const Action() const;
	
};