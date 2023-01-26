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
		compressedPercentage =(double)std::filesystem::file_size(filePath) / (double)(contents.size() * sizeof(uint16_t));
	}
	LzwFile(std::vector<uint16_t> compressedContents, std::size_t checkSum) : checkSum(checkSum)// to do later , fix the compressed percentage
	{
		contents = compressedContents;

	}
	LzwFile(const LzwFile& other) : filePath(other.filePath),contents(other.contents),
	compressedPercentage(other.compressedPercentage),checkSum(other.checkSum)
	{

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
	double compressedPercentage;
	std::size_t checkSum{ 0 };
	Compressor comp;
};