#include "LzwFile.h"

/**
 * @brief - reads a file byte by byte and stores the contents in a string
 * @param filePath  - path of the target file
 * @return  contents of the file stored in a string
*/
std::string LzwFile::readFileContents(std::filesystem::path filePath)
{
	
	std::ifstream file;
	std::string fileContents;
	try
	{
		file.open(filePath, std::ios::binary);
		std::size_t fileSize = std::filesystem::file_size(filePath);
		//std::string buffer(std::istreambuf_iterator<char>{file}, {});
		std::string buffer;
		buffer.resize(fileSize+1);
		file.read(buffer.data(), sizeof(char) * fileSize);
		buffer[fileSize] = '\0';
		fileContents = buffer;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
	return fileContents;
}
/**
 * @brief - constructor for a Lzw file by a given path
 * @param filePath - path to the uncompressed file
*/
LzwFile::LzwFile(std::filesystem::path filePath) : filePath(filePath)
{
	std::string fileContents = readFileContents(filePath);
	if(fileContents.empty())
	{
		compressedPercentage = 0;
		checkSum = 0;

	}
	else
	{
		contents = comp.compress(fileContents);
		checkSum = contents[contents.size() - 1];
		contents.pop_back();
		compressedPercentage = 100.0 * ((double)(contents.size() * sizeof(uint16_t)) / (double)std::filesystem::file_size(filePath));
	}
}
/**
 * @brief - constructor for  lzw file by given compressed data for the file
 * @param filePath - path in the archive to the file
 * @param perc - percentage of compression
 * @param compressedContents - compressed data
 * @param checkSum - verification for the data
*/
LzwFile::LzwFile(std::filesystem::path filePath, uint16_t perc, std::vector<uint16_t> compressedContents, std::size_t checkSum) : 
 filePath(filePath), contents(compressedContents), checkSum(checkSum), compressedPercentage(perc)
{}
/**
 * @brief - copy constructor
 * @param other - Lzw file
*/
LzwFile::LzwFile(const LzwFile & other) : filePath(other.filePath), contents(other.contents),
compressedPercentage(other.compressedPercentage), checkSum(other.checkSum)
{}
/**
 * @brief - assignment operator
 * @param other - Lzw file
 * @return - just this if the files are the same otherwise copies the contents and returns this
*/
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
std::filesystem::path LzwFile::getFilePath() const
{
	return filePath;
}
void LzwFile::setFilePath(std::filesystem::path filePath)
{
	this->filePath = filePath;
}
std::vector<uint16_t> LzwFile::getContents() const
{
	return contents;
}
uint16_t LzwFile::getPercentage() const
{
	return compressedPercentage;
}
std::size_t LzwFile::getCheckSum() const
{
	return checkSum;
}
/**
 * @brief - checks if the verification indicator is correct
 * @return - True if they are the same , false otherwise
*/
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
