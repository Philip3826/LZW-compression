#include "library.h"

void compress(std::string& fileAddress)
{
	
}

std::string extractFilePath(std::string& fileAddress)
{
	size_t indexlastSlash = fileAddress.find_last_of("\\");
	return fileAddress.substr(0,indexlastSlash);
	
}

