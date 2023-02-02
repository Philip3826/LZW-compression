#pragma once
#include <filesystem>
#include <vector>
#include <functional>
#include "Compressor.h"
class LzwFile {
	std::string readFileContents(std::filesystem::path filePath);
	std::filesystem::path filePath;
	std::vector<uint16_t> contents;
	uint16_t compressedPercentage;
	std::size_t checkSum{ 0 };
public:
	LzwFile(std::filesystem::path filePath);
	LzwFile(std::filesystem::path filePath, uint16_t perc, std::vector<uint16_t> compressedContents, std::size_t checkSum);
	~LzwFile() = default;
	LzwFile(const LzwFile& other);
	LzwFile& operator=(const LzwFile& other);
	std::filesystem::path getFilePath() const;
	void setFilePath(std::filesystem::path filePath);
	std::vector<uint16_t> getContents() const;
	uint16_t getPercentage() const;
	std::size_t getCheckSum() const;
	bool correctCheckSum();
	Compressor comp;
};