#pragma once
#include <filesystem>
#include <sstream>
#include <vector>
#include "LzwFile.h"
class ArchiveHandler {

public:
	void createArchive(std::filesystem::path newArchivePath, std::vector<std::filesystem::path> files);
	void extractArchive(std::filesystem::path destination, std::filesystem::path archive);
	void unzipFile(std::filesystem::path pathInArchive, std::ifstream& archive, std::filesystem::path  destination);
private:
	void compressDirectory(std::filesystem::path root,std::filesystem::path directoryPath,std::ofstream& archive);
	void writeFileBlock(LzwFile file, std::ofstream& archive);
	LzwFile readFileBlock(std::filesystem::path filelPath ,std::ifstream& archive);
	void unzipDirectory(std::filesystem::path destination, std::ifstream& archive);
	std::size_t findFile(std::filesystem::path filePath, std::ifstream& archive);
};