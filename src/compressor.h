#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
class Compressor 
{
public:
	std::vector<uint16_t> compress(std::filesystem::path filePath);
	std::string decompress(std::filesystem::path filePath, std::vector<uint16_t> compressedContent);
private:
	std::string readFileContents(std::filesystem::path filePath);
	void initializeDictionary(std::unordered_map<std::string,uint16_t>& dictionary);
	void initializeDecompressDictionary(std::unordered_map<uint16_t, std::string>& dictionary);
};