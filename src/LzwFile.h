#pragma once
#include <filesystem>
#include <vector>
#include "Compressor.h"
struct LzwFile {

	LzwFile(std::filesystem::path filePath) : filePath(filePath)
	{
		contents = comp.compress(filePath);
		compressedPercentage =(double)std::filesystem::file_size(filePath) / (double)(contents.size() * sizeof(uint16_t));
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
	Compressor comp;
};