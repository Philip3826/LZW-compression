#include "catch2/catch_all.hpp"
#include "ArchiveHandler.h"
#include "Compressor.h"
#include "ActionHandler.h"
#include <filesystem>

TEST_CASE("Action handler works properly")
{
	SECTION("Action handler correctly parses the command")
	{
		ActionHandler handler;
		std::string error = "some random string";
		handler.parseCommand(error);
		REQUIRE(handler.getAction() == ActionHandler::Action::ERROR);
		std::string zip = "zip";
		handler.parseCommand(zip);
		REQUIRE(handler.getAction() == ActionHandler::Action::ZIP);
		std::string unzip = "unzip";
		handler.parseCommand(unzip);
		REQUIRE(handler.getAction() == ActionHandler::Action::UNZIP);
		std::string refresh = "refresh";
		handler.parseCommand(refresh);
		REQUIRE(handler.getAction() == ActionHandler::Action::REFRESH);
		std::string ec = "ec";
		handler.parseCommand(ec);
		REQUIRE(handler.getAction() == ActionHandler::Action::EC);
		std::string info = "info";
		handler.parseCommand(info);
		REQUIRE(handler.getAction() == ActionHandler::Action::FILEINFO);
	}
	SECTION("Action handler correctly throws when validating the arguments")
	{
		int argc = 2;
		char** argv = nullptr;
		ActionHandler handler;
		REQUIRE_THROWS_AS(handler.validateArguments(argc,argv), std::invalid_argument);

	}
}

