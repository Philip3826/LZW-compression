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
	std::filesystem::path emp("C:\\Users\\phili\\Desktop\\empty");
	std::filesystem::path arch("C:\\Users\\phili\\Desktop\\dec.txt");
	std::vector<std::filesystem::path> v{one,two,three,emp};
	
	//handler.createArchive(arch, v);
	LzwFile file(three);
	std::cout << file.checkSum;
	//handler.extractArchive("C:\\Users\\phili\\Desktop", arch);
	return 0;
}