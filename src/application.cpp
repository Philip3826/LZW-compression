#include "library.h"
#include "Compressor.h"
#include "LzwFile.h"
#include "ArchiveHandler.h"
#include <iostream>
int main()
{
	ArchiveHandler handler;
	std::filesystem::path one("C:\\Users\\phili\\Desktop\\mc.txt");
	std::filesystem::path two("C:\\Users\\phili\\Desktop\\themes.txt");
	std::filesystem::path three("C:\\Users\\phili\\Desktop\\dump.txt");
	std::filesystem::path four("C:\\Users\\phili\\Desktop\\Bulgarian.txt");
	std::filesystem::path emp("C:\\Users\\phili\\Desktop\\empty");
	std::filesystem::path arch("C:\\Users\\phili\\Desktop\\dec.txt");
	std::filesystem::path ext("C:\\Users\\phili\\Desktop\\extract");
	std::vector<std::filesystem::path> v{one,two,three,emp,four};
	std::ifstream archive(arch, std::ios::binary);
	handler.createArchive(arch, v);
	//handler.unzipFile("empty", archive, ext); //error when searching folders
	handler.extractArchive("C:\\Users\\phili\\Desktop\\extract", arch);
	return 0;
}