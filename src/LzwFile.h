#pragma once
#include <filesystem>
#include <vector>
#include <functional>
#include "Compressor.h"
class LzwFile {

public:
	LzwFile(std::filesystem::path filePath);
	LzwFile(std::filesystem::path filePath, uint16_t perc, std::vector<uint16_t> compressedContents, std::size_t checkSum);
	LzwFile(const LzwFile& other);
	LzwFile& operator=(const LzwFile& other);
	bool correctCheckSum();
	std::filesystem::path filePath;
	std::vector<uint16_t> contents;
	uint16_t compressedPercentage;
	std::size_t checkSum{ 0 };
	Compressor comp;
};