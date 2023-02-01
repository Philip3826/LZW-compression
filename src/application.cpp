#include "Compressor.h"
#include "LzwFile.h"
#include "ArchiveHandler.h"
#include "ActionHandler.h"
#include <iostream>
int main(int argc , char** argv)
{
	//ArchiveHandler handler;
	//std::filesystem::path one("C:\\Users\\phili\\Desktop\\mc.txt");
	//std::filesystem::path two("C:\\Users\\phili\\Desktop\\themes.txt");
	//std::filesystem::path three("C:\\Users\\phili\\Desktop\\dump.txt");
	//std::filesystem::path four("C:\\Users\\phili\\Desktop\\Bulgarian.txt");
	//std::filesystem::path five("C:\\Users\\phili\\Desktop\\cppDump.txt");
	//std::filesystem::path six("C:\\Users\\phili\\Desktop\\mc2.txt");
	//std::filesystem::path emp("C:\\Users\\phili\\Desktop\\empty");
	//std::filesystem::path arch("C:\\Users\\phili\\Desktop\\arch.lzw");
	//std::filesystem::path ext("C:\\Users\\phili\\Desktop\\extract");

	//std::vector<std::filesystem::path> v{one,two,three,emp,four,five,six};
	////handler.createArchive(arch, v);
	//////handler.unzipFile("empty", archive, ext); //error when searching folders
	////handler.printInfo(arch);
	//////handler.checkForCorruption(arch);
	//handler.extractArchive(ext, arch);
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

	try
	{
		std::string command = *(argv + 1);
		handler.parseCommand(command);
		handler.validateArguments(argc, argv);
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