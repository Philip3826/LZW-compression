#include "Compressor.h"

std::vector<uint16_t> Compressor::compress(std::filesystem::path filePath)
{
	std::string fileContents = readFileContents(filePath);
	std::unordered_map<std::string, uint16_t> dictionary;
	initializeDictionary(dictionary);
	uint16_t nextKey = 256;
	std::string currStr = std::string(1,fileContents[0]);
	std::vector<uint16_t> compressedContents;
	for (std::string::iterator it = fileContents.begin() + 1; it != fileContents.end(); it++)
	{
		std::string buffer = currStr + *it;
		if (dictionary.contains(buffer)) currStr += *it;
		else
		{
			 auto output = dictionary.find(currStr);
			 std::cout << output->second << " ";
			 compressedContents.push_back(output->second);
			 if (nextKey < 4096)
			 {
				 dictionary.insert({ buffer,nextKey });
				 nextKey++;
			 }
			 currStr = *it;
		}
	}
	auto output = dictionary.find(currStr);
	std::cout << output->second;
	compressedContents.push_back(output->second);
	return compressedContents;
}

std::string Compressor::decompress(std::filesystem::path filePath, std::vector<uint16_t> compressedContent)
{
	
	std::unordered_map<uint16_t, std::string> dictionary;
	initializeDecompressDictionary(dictionary);
	uint16_t current = compressedContent[0];
	uint16_t code = 256;
	std::string decompressedContent;
	std::string translation = dictionary.find(current)->second;
	decompressedContent += translation;

	for (std::vector<uint16_t>::iterator it = compressedContent.begin() + 1; it != compressedContent.end(); it++)
	{
		current = (uint16_t)*it;
		std::string translationResult;
		auto keyValuePair = dictionary.find(current);

		if (keyValuePair != dictionary.end()) translationResult = keyValuePair->second;
		else translationResult = translation + translation[0]; // in case we encounter a code not yet in the dict

		dictionary.insert({ code++, translation + translationResult[0] });
		translation = translationResult;
		decompressedContent += translationResult;
		
	}
	return decompressedContent;
}

std::string Compressor::readFileContents(std::filesystem::path filePath)
{
	std::ifstream file(filePath, std::ios::binary);
	std::string fileContents(std::istreambuf_iterator<char>{file}, {});

	return fileContents;
}

void Compressor::initializeDictionary(std::unordered_map<std::string,uint16_t>& dictionary)
{
	for (uint16_t i = 0; i < 256; i++)
		dictionary.insert({ std::string(1, (char)i), i });
}

void Compressor::initializeDecompressDictionary(std::unordered_map<uint16_t, std::string>& dictionary)
{
	for (uint16_t i = 0; i < 256; i++)
		dictionary.insert({ i, std::string(1, (char)i) });
}
