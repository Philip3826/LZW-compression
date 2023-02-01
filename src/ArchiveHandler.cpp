#include "ArchiveHandler.h"
/**
 * @brief - creates an archive in a single file
 * @param newArchivePath - path to the archive file
 * @param files - list of files to be compressed
*/
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
			catch (const std::exception& e)
			{
				std::cerr << "Error:" << e.what() << '\n';
				return;
			}
		}
		if( std::filesystem::is_directory(file))
		{
			try
			{
				compressDirectory(file.filename(),file, archive);
			}
			catch(const std::exception& e)
			{
				std::cerr << "Error:" << e.what() << '\n';
				return;
			}
		}
	}
}
/**
 * @brief - decompressed the whole archive into the files it was built from
 * @param destination - where the decompressed files will go ( must be a directory)
 * @param archivePath - path to the archive
*/
void ArchiveHandler::extractArchive(std::filesystem::path destination, std::filesystem::path archivePath)
{
	std::ifstream archive(archivePath, std::ios::binary);
	if (!archive.is_open())
		throw std::runtime_error("Could not open archive!");

	std::size_t fileSize = std::filesystem::file_size(archivePath);
	try
	{
		while (archive.tellg() != fileSize)
		{
			std::size_t nameLength;
			std::string name;
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
				unzipDirectory(newDir, pathInArchive, archive, fileSize);
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
	catch (const std::exception& e)
	{
		std::cerr << "Error:" << e.what() << '\n';
		return;
	}
}

/**
 * @brief - unzips a single file from the archive.
 * @param pathInArchive - path of the file relative to the archive structure. Example : folder1\folder2\file.txt
 * @param archive - archive which we will be searching and extracting the file from
 * @param destination - where we will store the decompressed file
*/
void ArchiveHandler::unzipFile(std::filesystem::path pathInArchive, std::filesystem::path archivePath,std::filesystem::path destination)
{
	std::ifstream archive(archivePath, std::ios::binary);
	if (!archive.is_open()) throw std::runtime_error("Error with the compression");
	try
	{
		std::size_t readLocationStart = findFile(pathInArchive, archive);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error:" << e.what() << '\n';
		return;
	}
	bool isDir;
	try
	{
		archive.read(reinterpret_cast<char*>(&isDir), sizeof(bool));
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error:" << e.what() << '\n';
		return;
	}
	if (isDir)
	{
		std::filesystem::path newDir = destination / pathInArchive.filename();
		std::filesystem::create_directory(newDir);
		try
		{
			unzipDirectory(newDir,pathInArchive,archive,std::filesystem::file_size(archivePath)); // add arg
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error:" << e.what() << '\n';
			return;
		}
	}
	else
	{
		try
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
		catch (const std::exception& e)
		{
			std::cerr << "Error:" << e.what() << '\n';
			return;
		}
	}
}
/**
 * @brief - prints info about the structure of the archive as well as how well the files are compressed
 * @param archivePath 
*/
void ArchiveHandler::printInfo(std::filesystem::path archivePath)
{
	std::ifstream archive(archivePath, std::ios::binary);
	if (!archive.is_open())
		throw std::runtime_error("Error opening the archive");
	
	std::size_t fileSize = std::filesystem::file_size(archivePath);
	try
	{
		while (archive.tellg() != fileSize)
		{
			std::size_t nameLength;
			std::string name;
			archive.read(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
			name.resize(nameLength);
			archive.read(const_cast<char*>(name.c_str()), nameLength * sizeof(char));
			bool isDir;
			archive.read(reinterpret_cast<char*>(&isDir), sizeof(bool));
			if (isDir)
			{
				archive.seekg(sizeof(bool), std::ios::cur);
				std::cout << "Type:Folder, Name: " << name << std::endl;
			}
			else
			{
				uint16_t comprPerc;
				archive.read(reinterpret_cast<char*>(&comprPerc), sizeof(uint16_t));
				archive.seekg(sizeof(size_t), std::ios::cur); //skipping checkSum
				std::size_t contentLength;
				archive.read(reinterpret_cast<char*>(&contentLength), sizeof(size_t));
				archive.seekg(contentLength * sizeof(uint16_t), std::ios::cur); // skipping content
				std::cout << "Type:File, Name: " << name << ", Compressed percentage: " << comprPerc << std::endl;
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error:" << e.what() << '\n';
		return;
	}
}

/**
 * @brief - iterates through the archive checking for corruption by summing the codes and comparing the sum with the checkSum of the file
 * @param archivePath - path to the archive
*/
void ArchiveHandler::checkForCorruption(std::filesystem::path archivePath)
{
	std::ifstream archive(archivePath, std::ios::binary);
	if (!archive.is_open())
		throw std::runtime_error("Error opening the archive");
	std::size_t fileSize = std::filesystem::file_size(archivePath);
	bool noCorruptions = true;
	try
	{
		while (fileSize != archive.tellg())
		{
			std::size_t nameLength;
			std::string name;
			archive.read(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
			name.resize(nameLength);
			archive.read(const_cast<char*>(name.c_str()), nameLength * sizeof(char));
			bool isDir;
			archive.read(reinterpret_cast<char*>(&isDir), sizeof(bool));
			std::filesystem::path pathInArchive = name;
			if (isDir)
			{
				archive.seekg(sizeof(bool), std::ios::cur);
			}
			else
			{
				LzwFile file = readFileBlock(pathInArchive, archive);
				if (!file.correctCheckSum())
				{
					std::cout << "Corrupted file: " << name << std::endl;
					noCorruptions = false;
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << '\n';
		return;
	}

	if (noCorruptions) std::cout << "File is good" << std::endl;
}
/**
 * @brief - replaces a file in the archive with a new one or a directory with a new directory
 * @param newFile  - path to the new file for compression
 * @param pathInArchive - path to the file for replacement in the archive
 * @param archivePath - path to the archive
*/
void ArchiveHandler::refreshFile(std::filesystem::path newFile, std::filesystem::path pathInArchive, std::filesystem::path archivePath)
{
	std::ifstream archiveForReading(archivePath, std::ios::binary);
	if (!archiveForReading.is_open())
		throw std::runtime_error("Error with opening the archive");
	std::size_t fileStart = std::filesystem::file_size(archivePath);
	try
	{
		 fileStart = findFile(pathInArchive,archiveForReading);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error:" << e.what() << '\n';
		return;
	}
	
	
	if (fileStart == std::filesystem::file_size(archivePath))
	{
		std::cout << "File not found in the archive: " << std::endl;
		return;
	}
	std::size_t contentsBeforeFileLength = fileStart - sizeof(size_t) - pathInArchive.string().length();
	std::string contentsBeforeFile;
	contentsBeforeFile.resize(contentsBeforeFileLength);
	archiveForReading.seekg(archiveForReading.beg);
	archiveForReading.read(const_cast<char*>(contentsBeforeFile.c_str()) , sizeof(char) * contentsBeforeFileLength);
	archiveForReading.seekg(fileStart, archiveForReading.beg);
	bool isDir;
	archiveForReading.read(reinterpret_cast<char*>(&isDir), sizeof(bool));
	if (isDir)
	{
		if (!std::filesystem::is_directory(newFile))
		{
			std::cout << "New file is not a directory";
			return;
		}

		std::size_t endOfDir = findEndOfDir(pathInArchive.string(), archivePath, archiveForReading);
		if (endOfDir == 0)
		{
			std::cout << "Couldn't find end of directory for refreshing";
			return;
		}
		std::string restOfFile;
		std::size_t restLength = std::filesystem::file_size(archivePath) - archiveForReading.tellg();
		restOfFile.resize(restLength);
		archiveForReading.read(const_cast<char*>(restOfFile.c_str()), sizeof(char) * restLength);
		archiveForReading.close();
		std::ofstream archiveForWriting(archivePath, std::ios::binary, std::ios::trunc);
		archiveForWriting.write(contentsBeforeFile.c_str(), sizeof(char) * contentsBeforeFile.length());
		compressDirectory(newFile.filename(), newFile, archiveForWriting);
		archiveForWriting.write(restOfFile.c_str(), sizeof(char) * restLength);
	}
	else
	{
		if (std::filesystem::is_directory(newFile))
		{
			std::cout << "New file is not a regular file: ";
			return;
		}
		archiveForReading.seekg(sizeof(size_t) + sizeof(uint16_t), std::ios::cur); //skipping compressedPercentage an checkSum
		std::size_t contentLength;
		archiveForReading.read(reinterpret_cast<char*>(&contentLength), sizeof(size_t));
		archiveForReading.seekg(contentLength * sizeof(uint16_t), std::ios::cur); // skipping content
		
		std::string restOfFile;
		std::size_t restLength = std::filesystem::file_size(archivePath) - archiveForReading.tellg();
		restOfFile.resize(restLength);
		archiveForReading.read(const_cast<char*>(restOfFile.c_str()), sizeof(char) * restLength);
		archiveForReading.close();
		LzwFile file(newFile);
		std::ofstream archiveForWriting(archivePath, std::ios::binary , std::ios::trunc);

		if (!archiveForWriting.is_open())
			throw std::runtime_error("Error with opening the archive");

		archiveForWriting.write(contentsBeforeFile.c_str(), sizeof(char) * contentsBeforeFile.length());
		file.filePath = pathInArchive.parent_path() / file.filePath.filename();
		writeFileBlock(file, archiveForWriting);
		archiveForWriting.write(restOfFile.c_str(), sizeof(char) * restLength);

	}

}


/**
 * @brief - compressed  a single directory and writes it into an archive
 * @param root - path in the archive of a particular file. Example: if we are compressing the folder ..\..\folder\file.txt in the archive we will write \foldrer\file.txt
 * @param directoryPath - path to the directory we want to compress
 * @param archive - archive we will write the directory in 
*/
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
			catch (const std::exception& e)
			{
				std::cerr << "Error: " << e.what() << '\n';
				return;
			}
		}
		
		if (std::filesystem::is_directory(file))
		{
			std::filesystem::path newParent = file.path();
			newParent = root / newParent.filename();
			try
			{
				compressDirectory(newParent,file.path(), archive);
			}
			catch (const std::exception& e)
			{
				std::cerr << "Error: " << e.what() << '\n';
			}
		}
	}
}
/**
 * @brief - writes a file content into an archive
 * @param file - file kept in Lzw format ready for writing
 * @param archive - archive we will write in
*/
void ArchiveHandler::writeFileBlock(LzwFile file, std::ofstream& archive)
{
	std::string name = file.filePath.string();
	std::size_t nameLength = name.length();
	std::size_t compressedLength = file.contents.size();
	bool isDir = false;
	
	archive.write(reinterpret_cast<const char*>(&nameLength), sizeof(size_t));
	archive.write(name.c_str(), sizeof(char) * nameLength);
	archive.write(reinterpret_cast<const char*>(&isDir), sizeof(bool));
	archive.write(reinterpret_cast<const char*>(&file.compressedPercentage), sizeof(uint16_t));
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
	uint16_t comprPerc;
	archive.read(reinterpret_cast<char*>(&comprPerc), sizeof(uint16_t));
	archive.read(reinterpret_cast<char*>(&checkSum), sizeof(size_t));
	archive.read(reinterpret_cast<char*>(&contentLength), sizeof(size_t));
	std::vector<uint16_t> compressedContents(contentLength);

	for (std::vector<uint16_t>::iterator ft = compressedContents.begin(); ft != compressedContents.end() ; ft++)
	{
		archive.read(reinterpret_cast<char*>(&*ft), sizeof(uint16_t));
	}
	return LzwFile(filePath, comprPerc, compressedContents, checkSum);
}

void ArchiveHandler::unzipDirectory(std::filesystem::path destination, std::filesystem::path pathInFolder, std::ifstream& archive,std::size_t archiveSize)
{
	if (!archive.is_open())
		throw std::runtime_error("Error opening the archive!");
	bool isEmpty;
	archive.read(reinterpret_cast<char*>(&isEmpty), sizeof(bool));
	if (isEmpty) return;
	
	while (archive.tellg() != archiveSize)
	{
		std::size_t nameLength;
		std::string name;
		std::size_t positionBeforeName = archive.tellg();
		archive.read(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
		name.resize(nameLength);
		archive.read(const_cast<char*>(name.c_str()), nameLength * sizeof(char));
		std::filesystem::path pathInArchive = name;
		std::string path = pathInArchive.parent_path().string();
		if (path != pathInFolder) 
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
			unzipDirectory(newDir, pathInFolder / pathInArchive.filename(), archive,archiveSize); 
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

	while (!archive.eof())
	{
		std::size_t nameLength;
		archive.read(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
		std::string name;
		name.resize(nameLength);
		archive.read(const_cast<char*>(name.c_str()), nameLength * sizeof(char));

		if (filePath.string() == name.c_str()) return archive.tellg();
		bool isDir;
		archive.read(reinterpret_cast<char*>(&isDir), sizeof(bool));
		if (isDir)
		{
			archive.seekg(sizeof(bool), std::ios::cur);
		}
		else
		{
			skipFileBlock(archive, archive.tellg());
		}

	}


	return archive.end;
}

std::size_t ArchiveHandler::findEndOfDir(std::string dirName, std::filesystem::path archivePath, std::ifstream& archive)
{
	if (!archive.is_open())
		throw std::runtime_error("Error with opening the archive");

	bool isEmpty;
	archive.read(reinterpret_cast<char*>(&isEmpty), sizeof(bool));
	if (isEmpty) return archive.tellg();

	while (archive.tellg() != std::filesystem::file_size(archivePath))
	{
		std::size_t nameLength;
		archive.read(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
		std::string name;
		name.resize(nameLength);
		archive.read(const_cast<char*>(name.c_str()), nameLength * sizeof(char));

		if (name.substr(0, dirName.length()) != dirName)
		{
			archive.seekg(-sizeof(size_t) - sizeof(char) * nameLength, std::ios::cur);
			return archive.tellg();
		}
		bool isDir;
		archive.read(reinterpret_cast<char*>(&isDir), sizeof(bool));
		if (isDir)
		{
			archive.seekg(sizeof(bool), std::ios::cur);
		}
		else
		{
			skipFileBlock(archive,archive.tellg());
		}

	}
	return 0;
}
/**
 * @brief - seeks over the contents of a file block
 * @param archive - target file
 * @param pos - position from which starts the content for skipping (after the name and isDir indicator)
*/
void ArchiveHandler::skipFileBlock(std::ifstream& archive,std::size_t pos)
{
	if (!archive.is_open())
		throw std::runtime_error("Error with opening the archive");

	archive.seekg(pos);
	archive.seekg(sizeof(uint16_t) + sizeof(size_t), std::ios::cur);
	std::size_t contentLength;
	archive.read(reinterpret_cast<char*>(&contentLength), sizeof(size_t));
	archive.seekg(contentLength * sizeof(uint16_t), std::ios::cur);
}
