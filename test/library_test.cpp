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
	SECTION("Action handler correctly throws when validating arguments")
	{
		ActionHandler handler;
		REQUIRE_THROWS_AS(handler.validateArguments(std::vector<std::string>()), std::invalid_argument);
		handler.parseCommand("zip");
		std::vector<std::string> args{ "archive.lzw" };
		REQUIRE_THROWS_AS(handler.validateArguments(args), std::invalid_argument);
		args.push_back("arg");
		REQUIRE_NOTHROW(handler.validateArguments(args));
		handler.parseCommand("unzip");
		REQUIRE_NOTHROW(handler.validateArguments(args));
		args.pop_back();
		REQUIRE_THROWS_AS(handler.validateArguments(args), std::invalid_argument);
		handler.parseCommand("info");
		REQUIRE_NOTHROW(handler.validateArguments(args));
		args.pop_back();
		REQUIRE_THROWS_AS(handler.validateArguments(args), std::invalid_argument);
		handler.parseCommand("refresh");
		REQUIRE_THROWS_AS(handler.validateArguments(args), std::invalid_argument);
		args.push_back("one.lzw");
		args.push_back("two");
		args.push_back("three");
		REQUIRE_NOTHROW(handler.validateArguments(args));
		args.push_back("four");
		REQUIRE_THROWS_AS(handler.validateArguments(args), std::invalid_argument);
		handler.parseCommand("ec");
		REQUIRE_THROWS_AS(handler.validateArguments(args), std::invalid_argument);
		args.clear();
		args.push_back("one.lzw");
		REQUIRE_NOTHROW(handler.validateArguments(args));
	}
}

TEST_CASE("Compressor works properly")
{
	Compressor comp;
	SECTION("compress works properly")
	{
		
		std::string empty= "";
		std::vector<uint16_t> result = comp.compress(empty);
		REQUIRE(result.empty());

		std::string simple = "abc";
		std::vector<uint16_t> simpleVector = { 'a','b','c'};
		result = comp.compress(simple);
		result.pop_back(); // removing the checkSum
		REQUIRE(result.size() == simpleVector.size());
		for (std::size_t i = 0; i < simpleVector.size(); i++)
			REQUIRE(result[i] == simpleVector[i]);

		std::string longer = "TOBEORNOTTOBEORTOBEORNOTTOBE";
		std::vector<uint16_t> longerVector = { 84,79,66,69,79,82,78,79,84,256,258,260,265,259,261,263,268 };
		result = comp.compress(longer);
		result.pop_back();
		REQUIRE(result.size() == longerVector.size());
		for (std::size_t i = 0; i < longerVector.size(); i++)
			REQUIRE(result[i] == longerVector[i]);
	}

	SECTION("compress correctly calculates checkSum")
	{
		std::string test = "TOBEORNOTTOBEORTOBEORNOTTOBE";
		std::vector<uint16_t> vector = { 84,79,66,69,79,82,78,79,84,256,258,260,265,259,261,263,268,(2790 % 65536) };
		std::vector<uint16_t> result = comp.compress(test);
		REQUIRE(result.back() == vector.back());
	}

	SECTION("decompress works properly")
	{
		std::vector<uint16_t> empty(0);
		std::string result = comp.decompress(empty);
		REQUIRE(result.empty());

		std::vector<uint16_t> simple{ 'a','b','c' };
		result = comp.decompress(simple);
		REQUIRE(result == "abc");

		std::vector<uint16_t> longer = { 84,79,66,69,79,82,78,79,84,256,258,260,265,259,261,263,268 };
		result = comp.decompress(longer);
		REQUIRE(result == "TOBEORNOTTOBEORTOBEORNOTTOBE");
	}
}

