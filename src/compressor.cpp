#include "Compressor.h"
/**
 * @brief - A function that compresses the contents of a file using the LZW algorithm
 * @param filePath - path to the file we want to compress
 * @return - vector containing the compressed content in the form of coded uint16_t numbers
*/
std::vector<uint16_t> Compressor::compress(std::string fileContents)
{
	if (fileContents.empty())
	{
		return std::vector<uint16_t>();
	}
	std::unordered_map<std::string, uint16_t> dictionary;
	initializeDictionary(dictionary);
	uint16_t nextKey = 256;
	std::string currStr(1,fileContents[0]); 
	std::vector<uint16_t> compressedContents;
	std::size_t totalSum = 0;
	std::string buffer;
	buffer.resize(100);
	for (std::string::iterator it = fileContents.begin() + 1; it != fileContents.end(); it++)
	{
		buffer.clear();
		buffer = currStr + *it;
		if (dictionary.contains(buffer)) {
			
			std::swap(buffer, currStr);
		}
		else
		{
			 auto output = dictionary.find(currStr);
			 compressedContents.push_back(output->second);
			 totalSum += output->second;
			 if (nextKey < 4096)
			 {
				 dictionary.insert({ buffer,nextKey });
				 nextKey++;
			 }
			 currStr = *it;
		}
	}
	auto output = dictionary.find(currStr);
	compressedContents.push_back(output->second);
	totalSum += output->second;
	compressedContents.push_back(totalSum % 65536);
	return compressedContents;
}
/**
 * @brief - decompresses compressed data using the LZW algorithm
 * @param filePath -  
 * @param compressedContent - vector with the compressed data from a file in the form of coded uint16_t numbers 
 * @return string with the decompressed contents
*/
std::string Compressor::decompress(std::vector<uint16_t> compressedContent)
{
	if (compressedContent.empty())
	{
		return std::string();
	}
	std::unordered_map<uint16_t, std::string> dictionary;
	initializeDecompressDictionary(dictionary);
	uint16_t current = compressedContent[0];
	uint16_t code = 256;
	std::string decompressedContent;
	std::string oldTranslation = dictionary.find(current)->second; // changed name 
	decompressedContent += oldTranslation;

	for (std::vector<uint16_t>::iterator it = compressedContent.begin() + 1; it != compressedContent.end(); it++)
	{
		current = *it;
		std::string translationResult;
		auto keyValuePair = dictionary.find(current);

		if (keyValuePair != dictionary.end()) translationResult = keyValuePair->second;
		else translationResult = oldTranslation + oldTranslation[0]; // in case we encounter a code not yet in the dict

		dictionary.insert({ code++, oldTranslation + translationResult[0] });
		oldTranslation = translationResult;
		decompressedContent += translationResult;
		
	}
	return decompressedContent;
}


/**
 * @brief - initalizes the ascii table into the dictionary used for compression
 * @param dictionary  - dictionary used in the compression
*/
void Compressor::initializeDictionary(std::unordered_map<std::string,uint16_t>& dictionary)
{
	for (uint16_t i = 0; i < 256; i++)
		dictionary.insert({ std::string(1, (char)i), i });
}
/**
 * @brief - initalizes the ascii table into the dictionary used for compression
 * @param dictionary - dictionary used in the decompression
*/
void Compressor::initializeDecompressDictionary(std::unordered_map<uint16_t, std::string>& dictionary)
{
	for (uint16_t i = 0; i < 256; i++)
		dictionary.insert({ i, std::string(1, (char)i) });
}
