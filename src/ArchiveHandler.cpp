#include "ArchiveHandler.h"

void ArchiveHandler::createArchive(std::filesystem::path newArchivePath, std::vector<std::filesystem::path> files)
{
	std::ofstream archived(newArchivePath, std::ios::binary | std::ios::trunc);
	for (std::vector<std::filesystem::path>::iterator it = files.begin(); it != files.end(); it++)
	{
		if (std::filesystem::is_regular_file(*it))
		{
			LzwFile compressedCurrent(*it);
			std::size_t nameLength = compressedCurrent.filePath.string().length();
			std::string name = compressedCurrent.filePath.string();
			std::size_t compressedLength = compressedCurrent.contents.size();
			//std::cout << nameLength << " ";
			archived.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
			archived.write(name.c_str(), sizeof(char) * nameLength);
			archived.write(reinterpret_cast<const char*>(&compressedLength), sizeof(compressedLength));
			for (std::vector<uint16_t>::iterator ft = compressedCurrent.contents.begin(); ft != compressedCurrent.contents.end(); ft++)
			{
				archived.write(reinterpret_cast<const char*>(&*ft), sizeof(uint16_t));
			}
			
		}
	}
	archived.close();
}

void ArchiveHandler::extractArchive(std::filesystem::path destination, std::filesystem::path archivePath)
{
	std::ifstream archive(archivePath, std::ios::binary);
	if (!archive.is_open())
		throw std::runtime_error("Could not open archive!");

	while (!archive.eof())
	{
		std::size_t nameLength;
		archive.read(reinterpret_cast<char*> (&nameLength), sizeof(size_t));
		std::string name;
		char* buffer = new char[nameLength];
		archive.read(buffer, nameLength * sizeof(char));
		name.assign(buffer);
		delete buffer;
		std::cout  << name << " ";
		std::size_t contentLength;
		archive.read(reinterpret_cast<char*>(&contentLength), sizeof(size_t));
		std::vector<uint16_t> compressedContents(contentLength);
		for (std::vector<uint16_t>::iterator ft = compressedContents.begin(); ft != compressedContents.end() ; ft++)
		{
			archive.read(reinterpret_cast<char*>(&*ft), sizeof(uint16_t));
		}
		LzwFile file(compressedContents);
		std::cout << file.comp.decompress(archivePath,compressedContents);
	}
}
