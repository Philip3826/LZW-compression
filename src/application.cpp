#include "Compressor.h"
#include "LzwFile.h"
#include "ArchiveHandler.h"
#include "ActionHandler.h"
#include <iostream>
int main(int argc , char** argv)
{
	ArchiveHandler handler;
	std::filesystem::path one("C:\\Users\\phili\\Desktop\\mc.txt");
	std::filesystem::path two("C:\\Users\\phili\\Desktop\\themes.txt");
	std::filesystem::path three("C:\\Users\\phili\\Desktop\\dump.txt");
	std::filesystem::path four("C:\\Users\\phili\\Desktop\\Bulgarian.txt");
	std::filesystem::path five("C:\\Users\\phili\\Desktop\\cppDump.txt");
	std::filesystem::path six("C:\\Users\\phili\\Desktop\\mc2.txt");
	std::filesystem::path emp("C:\\Users\\phili\\Desktop\\empty");
	std::filesystem::path arch("C:\\Users\\phili\\Desktop\\dec.txt");
	std::filesystem::path ext("C:\\Users\\phili\\Desktop\\extract");

	//std::vector<std::filesystem::path> v{one,two,three,emp,four,five,six};
	////handler.createArchive(arch, v);
	////handler.unzipFile("empty", archive, ext); //error when searching folders
	//handler.printInfo(arch);
	////handler.checkForCorruption(arch);
	//handler.refreshFile(emp, "empty", arch);
	//handler.extractArchive(ext, arch);
	std::filesystem::path edno(argv[3]);
	std::filesystem::path dve(argv[4]);
	std::vector<std::filesystem::path> vv{ edno,dve };
	handler.createArchive(argv[2], vv);
	return 0;
}