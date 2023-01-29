#pragma once
#include <filesystem>
#include <vector>
#include <functional>
#include "Compressor.h"
class LzwFile {

public:
	LzwFile(std::filesystem::path filePath) : filePath(filePath)
	{
		contents = comp.compress(filePath);
		checkSum = contents[contents.size() - 1];
		contents.pop_back();
		compressedPercentage = 100.0 * ((double)(contents.size() * sizeof(uint16_t)) / (double)std::filesystem::file_size(filePath));
		std::cout << filePath << " " << (contents.size() * sizeof(uint16_t)) << " " << std::filesystem::file_size(filePath) << '\n';
	}
	LzwFile(std::filesystem::path filePath,double perc,std::vector<uint16_t> compressedContents, std::size_t checkSum) : 
		filePath(filePath),contents(compressedContents),checkSum(checkSum),compressedPercentage(perc)
	{}
	LzwFile(const LzwFile& other) : filePath(other.filePath),contents(other.contents),
	compressedPercentage(other.compressedPercentage),checkSum(other.checkSum)
	{}
	LzwFile& operator=(const LzwFile& other)
	{
		if (this != &other)
		{
			filePath = other.filePath;
			contents = other.contents;
			compressedPercentage = other.compressedPercentage;
			checkSum = other.checkSum;
		}
	}
	void print()
	{
		for (std::vector<uint16_t>::iterator it = contents.begin(); it != contents.end(); it++)
		{
			std::cout << *it << " ";
		}
	}
	std::filesystem::path filePath;
	std::vector<uint16_t> contents;
	uint16_t compressedPercentage;
	std::size_t checkSum{ 0 };
	Compressor comp;
};