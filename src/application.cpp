#include "Compressor.h"
#include "LzwFile.h"
#include "ArchiveHandler.h"
#include "ActionHandler.h"
#include <iostream>
int main(int argc , char** argv)
{
	std::string usage = "1.To archive files please input : ./app.exe zip address\\to\\archive.lzw [address\\to\\file]\n\
2.To decompress files from an archive please input: ./app.exe unzip address\\to\\archive.lzw address\\to\\unzip\\destination [address\\in\\archive\\to\\file]\n\
3.To decompress the whole archive please input: ./app.exe unzip address\\to\\archive.lzw address\\to\\unzip\\destination\n\
4.To print info about the archive structure please input: ./app.exe info address\\to\\archive.lzw\n\
5.To check if file is not corrupted please input: ./app.exe ec address\\to\\archive.lzw\n\
6.To refresh a file or folder in the archive with a new one please input: ./app.exe refresh address\\to\\archive.lzw path\\to\\new\\file path\\in\\archive\\to\\file";

	ActionHandler handler;
	if (argc < 2)
	{
		std::cout << usage;
		return 1;
	}
	std::vector<std::string> arguments(argv + 2, argv + argc);
	std::string command = *(argv + 1);
	try
	{
		handler.parseCommand(command);
		handler.validateArguments(arguments);
		handler.executeAction();
	}
	catch (std::invalid_argument& i)
	{
		std::cout << i.what() << std::endl;
		std::cout << usage;
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}

	return 0;
}