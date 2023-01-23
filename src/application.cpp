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
	std::filesystem::path arch("C:\\Users\\phili\\Desktop\\dec.txt");
	std::vector<std::filesystem::path> v{one,two,three};
	handler.createArchive(arch, v);
	handler.extractArchive("C:\\Users\\phili\\Desktop", arch);
	return 0;
}