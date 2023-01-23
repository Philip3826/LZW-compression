#pragma once
#include <filesystem>
#include <sstream>
#include <vector>
#include "LzwFile.h"
class ArchiveHandler {

public:
	void createArchive(std::filesystem::path newArchivePath, std::vector<std::filesystem::path> files);
	void extractArchive(std::filesystem::path destination, std::filesystem::path archive);
private:

};