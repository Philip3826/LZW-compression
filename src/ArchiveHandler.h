#pragma once
#include <filesystem>
#include <sstream>
#include <vector>
#include "LzwFile.h"
class ArchiveHandler {

public:
	void createArchive(std::filesystem::path newArchivePath, std::vector<std::filesystem::path> files);
	void extractArchive(std::filesystem::path destination, std::filesystem::path archive);
	void unzipFile(std::filesystem::path pathInArchive, std::filesystem::path archivePath, std::filesystem::path  destination);
	void printInfo(std::filesystem::path archivePath);
	void checkForCorruption(std::filesystem::path archivePath);
	void refreshFile(std::filesystem::path newFile, std::filesystem::path pathInArchive, std::filesystem::path archivePath);
private:
	void compressDirectory(std::filesystem::path root,std::filesystem::path directoryPath,std::ofstream& archive);
	void writeFileBlock(LzwFile file, std::ofstream& archive);
	LzwFile readFileBlock(std::filesystem::path filelPath ,std::ifstream& archive);
	void unzipDirectory(std::filesystem::path destination,std::filesystem::path pathInFolder,std::ifstream& archive,std::size_t archiveSize);
	std::size_t findFile(std::filesystem::path filePath, std::ifstream& archive);
	std::size_t findEndOfDir(std::string dirName, std::filesystem::path archivePath,std::ifstream& archive);
	void skipFileBlock(std::ifstream& archive,std::size_t pos);
};