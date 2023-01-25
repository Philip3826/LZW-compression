#include "ArchiveHandler.h"

void ArchiveHandler::createArchive(std::filesystem::path newArchivePath, std::vector<std::filesystem::path> files)
{
	std::ofstream archive(newArchivePath, std::ios::binary | std::ios::trunc);
	for (std::filesystem::path file : files)
	{
		if (std::filesystem::is_regular_file(file))
		{
			LzwFile compressedCurrent(file);
			std::string name = compressedCurrent.filePath.filename().string();
			writeFileBlock(compressedCurrent, name, archive);
		}
		if( std::filesystem::is_directory(file))
		{
			compressDirectory(file.filename(),file, archive);
		}
	}
	archive.close();
}

void ArchiveHandler::extractArchive(std::filesystem::path destination, std::filesystem::path archivePath)
{
	std::ifstream archive(archivePath, std::ios::binary);
	if (!archive.is_open())
		throw std::runtime_error("Could not open archive!");

	while (!archive.eof())
	{
		std::size_t checkSum;
		archive.read(reinterpret_cast<char*>(&checkSum), sizeof(size_t));
		std::size_t nameLength;
		archive.read(reinterpret_cast<char*> (&nameLength), sizeof(size_t));
		std::string name;
		char* buffer = new char[nameLength];
		archive.read(buffer, nameLength * sizeof(char));
		name.assign(buffer);
		delete[] buffer;
		std::filesystem::path filePath(name);
		std::size_t contentLength;
		archive.read(reinterpret_cast<char*>(&contentLength), sizeof(size_t));
		std::vector<uint16_t> compressedContents(contentLength);
		for (std::vector<uint16_t>::iterator ft = compressedContents.begin(); ft != compressedContents.end() ; ft++)
		{
			archive.read(reinterpret_cast<char*>(&*ft), sizeof(uint16_t));
		}
		LzwFile file(compressedContents,checkSum); // TO DO : make a verification on the checkSum
		std::cout << file.comp.decompress(archivePath,compressedContents);
	}
}
 // TO DO : change argument root name
void ArchiveHandler::compressDirectory(std::filesystem::path root , std::filesystem::path directoryPath , std::ofstream& archive)
{
	if (!archive.is_open())
		throw std::runtime_error("Error with the compression!");

	std::filesystem::path pathInArchive = root;
	std::cout << pathInArchive;
	std::size_t nameLength = pathInArchive.string().length();
	archive.write(reinterpret_cast<const char*>(&nameLength), sizeof(size_t));
	archive.write(pathInArchive.string().c_str(), sizeof(char) * nameLength);
		
	bool isEmpty = true;
	
	if (std::filesystem::is_empty(directoryPath))
	{
		// what checkSum should i do for empty dirs?
		isEmpty = 0;
		archive.write(reinterpret_cast<const char*>(&isEmpty), sizeof(bool));
		return;

	}
	else archive.write(reinterpret_cast<const char*>(&isEmpty), sizeof(bool));

	std::filesystem::directory_iterator fileIterator(directoryPath);
	
	for (auto file : fileIterator)
	{
		if (std::filesystem::is_regular_file(file))
		{
			LzwFile compressedCurrent(file);
			compressedCurrent.filePath = root / compressedCurrent.filePath.filename();
			writeFileBlock(compressedCurrent, compressedCurrent.filePath.string(), archive);
		}
		
		if (std::filesystem::is_directory(file))
		{
			std::filesystem::path newParent = file.path();
			newParent = root / newParent.filename();
			compressDirectory(newParent,file.path(), archive);
		}
	}
}
// To do : change it so we can get the name from the file
void ArchiveHandler::writeFileBlock(LzwFile file, std::string name ,std::ofstream& archive)
{
	std::size_t nameLength = name.length();
	std::size_t compressedLength = file.contents.size();
	std::size_t checkSum = (std::size_t)file.checkSum * 100;
	archive.write(reinterpret_cast<const char*>(&checkSum), sizeof(size_t));
	archive.write(reinterpret_cast<const char*>(&nameLength), sizeof(size_t));
	archive.write(name.c_str(), sizeof(char) * nameLength);
	archive.write(reinterpret_cast<const char*>(&compressedLength), sizeof(size_t));
	for (std::vector<uint16_t>::iterator ft = file.contents.begin(); ft != file.contents.end(); ft++)
	{
		archive.write(reinterpret_cast<const char*>(&*ft), sizeof(uint16_t));
	}
}
