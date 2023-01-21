#pragma once
#include <filesystem>
#include <vector>
#include "LzwFile.h"
class ArchiveHandler {

public:
	void createArchive(std::filesystem::path newArchivePath, std::vector<std::filesystem::path> files);
private:

};