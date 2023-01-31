#include "LzwFile.h"

LzwFile::LzwFile(std::filesystem::path filePath) : filePath(filePath)
{
	contents = comp.compress(filePath);
	checkSum = contents[contents.size() - 1];
	contents.pop_back();
	compressedPercentage = 100.0 * ((double)(contents.size() * sizeof(uint16_t)) / (double)std::filesystem::file_size(filePath));
}

LzwFile::LzwFile(std::filesystem::path filePath, uint16_t perc, std::vector<uint16_t> compressedContents, std::size_t checkSum) : 
 filePath(filePath), contents(compressedContents), checkSum(checkSum), compressedPercentage(perc)
{}

LzwFile::LzwFile(const LzwFile & other) : filePath(other.filePath), contents(other.contents),
compressedPercentage(other.compressedPercentage), checkSum(other.checkSum)
{}

LzwFile& LzwFile::operator=(const LzwFile & other)
{
	if (this != &other)
	{
		filePath = other.filePath;
		contents = other.contents;
		compressedPercentage = other.compressedPercentage;
		checkSum = other.checkSum;
	}
	return *this;
}

bool LzwFile::correctCheckSum()
{
	std::size_t codeSum = 0;
	for (uint16_t i : contents)
	{
		codeSum += i;
	}
	if (checkSum != codeSum % 65536) return false;

	return true;
}
