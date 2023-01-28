#include "ArchiveHandler.h"

void ArchiveHandler::createArchive(std::filesystem::path newArchivePath, std::vector<std::filesystem::path> files)
{
	std::ofstream archive(newArchivePath, std::ios::binary | std::ios::trunc);
	for (std::filesystem::path file : files)
	{
		if (std::filesystem::is_regular_file(file))
		{
			LzwFile compressedCurrent(file);
			compressedCurrent.filePath = compressedCurrent.filePath.filename();
			try 
			{
				writeFileBlock(compressedCurrent, archive);
			}
			catch (std::ios_base::failure& e)
			{
				std::cerr << "Error with writing in the file" << e.what() << '\n';
				return;
			}
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
	std::size_t fileSize = std::filesystem::file_size(archivePath);

	while (archive.tellg() != fileSize)
	{
		std::size_t nameLength;
		std::string name;
		std::cout << archive.tellg() << " ";
		archive.read(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
		name.resize(nameLength);
		archive.read(const_cast<char*>(name.c_str()), nameLength * sizeof(char));
		bool isDir;
		archive.read(reinterpret_cast<char*>(&isDir), sizeof(bool));
		std::filesystem::path pathInArchive = name;
		if (isDir)
		{
			std::filesystem::path newDir = destination / pathInArchive.filename();
			std::filesystem::create_directory(newDir);
			unzipDirectory(newDir, pathInArchive, archive);
		}
		else
		{
			LzwFile file = readFileBlock(pathInArchive, archive);
			std::string decompressed = file.comp.decompress(file.contents);
			std::filesystem::path newAddress = destination / pathInArchive.filename();
			std::ofstream newFile(newAddress, std::ios::binary | std::ios::trunc);
			if (newFile.good())
			{
				newFile.write(decompressed.c_str(), sizeof(char) * decompressed.length());
				newFile.close();
			}
		}
	}
}

/**
 * @brief - unzips a single file from the archive.
 * @param pathInArchive - path of the file relative to the archive structure. Example : folder1\folder2\file.txt
 * @param archive - archive which we will be searching and extracting the file from
 * @param destination - where we will store the decompressed file
*/
void ArchiveHandler::unzipFile(std::filesystem::path pathInArchive, std::ifstream& archive,std::filesystem::path destination)
{
	if (!archive.is_open()) throw std::runtime_error("Error with the compression");
	std::size_t readLocationStart = findFile(pathInArchive, archive);
	bool isDir;
	archive.read(reinterpret_cast<char*>(&isDir), sizeof(bool));
	if (isDir)
	{
		std::filesystem::path newDir = destination / pathInArchive.filename();
		std::filesystem::create_directory(newDir);
		unzipDirectory(newDir,pathInArchive,archive); // add arg
	}
	else
	{
		LzwFile file = readFileBlock(pathInArchive, archive);
		
		std::string decompressed = file.comp.decompress(file.contents);
		std::filesystem::path newAddress = destination / pathInArchive.filename();
		std::ofstream newFile(newAddress, std::ios::binary | std::ios::trunc);
		if (newFile.good())
		{
			newFile.write(decompressed.c_str(), sizeof(char) * decompressed.length());
			newFile.close();
		}
		
	}
}


 // TO DO : change argument root name
void ArchiveHandler::compressDirectory(std::filesystem::path root , std::filesystem::path directoryPath , std::ofstream& archive)
{
	if (!archive.is_open())
		throw std::runtime_error("Error with the compression!");

	std::filesystem::path pathInArchive = root;
	std::size_t nameLength = pathInArchive.string().length();
	bool isDir = true;
	archive.write(reinterpret_cast<const char*>(&nameLength), sizeof(size_t));
	archive.write(pathInArchive.string().c_str(), sizeof(char) * nameLength);
	archive.write(reinterpret_cast<const char*>(&isDir), sizeof(bool));
	bool isEmpty = false;
	
	if (std::filesystem::is_empty(directoryPath))
	{
		// what checkSum should i do for empty dirs?
		isEmpty = true;
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
			try
			{
				writeFileBlock(compressedCurrent, archive);
			}
			catch (std::ios_base::failure& e)
			{
				std::cerr << "Error with writing in the file" << e.what() << '\n';
				return;
			}
		}
		
		if (std::filesystem::is_directory(file))
		{
			std::filesystem::path newParent = file.path();
			newParent = root / newParent.filename();
			compressDirectory(newParent,file.path(), archive);
		}
	}
}
// To do : add compressed percentage
void ArchiveHandler::writeFileBlock(LzwFile file, std::ofstream& archive)
{
	std::string name = file.filePath.string();
	std::size_t nameLength = name.length();
	std::size_t compressedLength = file.contents.size();
	bool isDir = false;
	
	archive.write(reinterpret_cast<const char*>(&nameLength), sizeof(size_t));
	archive.write(name.c_str(), sizeof(char) * nameLength);
	archive.write(reinterpret_cast<const char*>(&isDir), sizeof(bool));
	archive.write(reinterpret_cast<const char*>(&file.checkSum), sizeof(size_t));
	archive.write(reinterpret_cast<const char*>(&compressedLength), sizeof(size_t));
	for (std::vector<uint16_t>::iterator ft = file.contents.begin(); ft != file.contents.end(); ft++)
	{
		archive.write(reinterpret_cast<const char*>(&*ft), sizeof(uint16_t));
	}
}


LzwFile ArchiveHandler::readFileBlock(std::filesystem::path filePath,std::ifstream& archive)
{
	
	std::size_t checkSum;
	std::size_t contentLength;
	archive.read(reinterpret_cast<char*>(&checkSum), sizeof(size_t));
	archive.read(reinterpret_cast<char*>(&contentLength), sizeof(size_t));
	std::vector<uint16_t> compressedContents(contentLength);

	for (std::vector<uint16_t>::iterator ft = compressedContents.begin(); ft != compressedContents.end() ; ft++)
	{
		archive.read(reinterpret_cast<char*>(&*ft), sizeof(uint16_t));
	}
	return LzwFile(filePath, 0, compressedContents, checkSum);
}

void ArchiveHandler::unzipDirectory(std::filesystem::path destination, std::filesystem::path pathInFolder, std::ifstream& archive)
{
	bool isEmpty;
	archive.read(reinterpret_cast<char*>(&isEmpty), sizeof(bool));
	if (isEmpty) return;
	
	//std::size_t checksum;
	//archive.read(reinterpret_cast<char*>(&checksum),sizeof(size_t));
	while (!archive.eof())
	{
		std::size_t nameLength;
		std::string name;
		std::size_t positionBeforeName = archive.tellg();
		archive.read(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
		name.resize(nameLength);
		archive.read(const_cast<char*>(name.c_str()), nameLength * sizeof(char));
		std::filesystem::path pathInArchive = name;
		std::string path = pathInArchive.parent_path().string();
		if (path != pathInFolder) // fix this check
		{
			archive.seekg(positionBeforeName);
			break;
		}
		bool isDir;
		archive.read(reinterpret_cast<char*>(&isDir), sizeof(bool));
		if (isDir)
		{
			std::filesystem::path newDir = destination / pathInArchive.filename();
			std::filesystem::create_directory(newDir);
			unzipDirectory(newDir, pathInFolder / pathInArchive.filename(), archive); //add arg
		}
		else
		{
			LzwFile file = readFileBlock(pathInArchive, archive);
			std::string decompressed = file.comp.decompress(file.contents);
			std::filesystem::path newAddress = destination / pathInArchive.filename();
			std::ofstream newFile(newAddress, std::ios::binary | std::ios::trunc);
			if (newFile.good())
			{
				newFile.write(decompressed.c_str(), sizeof(char) * decompressed.length());
				newFile.close();
			}
		}
	}
	

}


/**
 * @brief - finds the starting position of the fileblock 
 * @param filePath - file we want to file
 * @param archive - archive which we travers
 * @return - position in which the fileblock starts, returns archive.end if we can't find the file
*/
std::size_t ArchiveHandler::findFile(std::filesystem::path filePath, std::ifstream& archive)
{
	if (!archive.is_open()) throw std::runtime_error("error with the archive!");
	//archive.seekg(0, archive.beg);

	while (!archive.eof())
	{
		std::size_t nameLength;
		archive.read(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
		std::string name;
		name.resize(nameLength);
		archive.read(const_cast<char*>(name.c_str()), nameLength * sizeof(char));

		//std::cout << "After name read:" << archive.tellg() << '\n';
		if (filePath.string() == name.c_str())
		{
			/*try
			{
				archive.seekg(-(sizeof(size_t) + nameLength*sizeof(char)), std::ios::cur);
			}
			catch (std::ios_base::failure& e)
			{
				std::cerr << "Error with seeking in the file" << e.what() << '\n';
				return archive.end;
			}*/
			return archive.tellg();
		}
		bool isDir;
		archive.read(reinterpret_cast<char*>(&isDir), sizeof(bool));
		if (isDir)
		{
			try
			{
				archive.seekg(sizeof(bool), std::ios::cur);
				// to do  add checksum here
			}
			catch (std::ios_base::failure& e)
			{
				std::cerr << "Error with seeking in the file" << e.what() << '\n';
				return archive.end;
			}
		}
		else
		{
			try
			{
				archive.seekg(sizeof(size_t), std::ios::cur); // skipping checkSum value
				std::size_t contentLength;
				archive.read(reinterpret_cast<char*>(&contentLength), sizeof(size_t));
				archive.seekg(contentLength * sizeof(uint16_t), std::ios::cur); // skipping content
			}
			catch (std::ios_base::failure& e)
			{
				std::cerr << "Error with seeking in the file" << e.what() << '\n';
				return archive.end;
			}
		}

	}


	return archive.end;
}
