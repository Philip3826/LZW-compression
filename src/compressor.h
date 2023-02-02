#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <functional>
class Compressor 
{
public:
	std::vector<uint16_t> compress(std::string fileContents);
	std::string decompress(std::vector<uint16_t> compressedContent);
private:
	void initializeDictionary(std::unordered_map<std::string,uint16_t>& dictionary);
	void initializeDecompressDictionary(std::unordered_map<uint16_t, std::string>& dictionary);
};