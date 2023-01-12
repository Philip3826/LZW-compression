#include "library.h"

//std::ofstream& compress(std::ofstream& file,std::string& fileAddress)
//{
//	
//	//std::ofstream out(, std::ios::binary);
//
//}

std::string extractFilePath(std::string& fileAddress)
{
	size_t indexlastSlash = fileAddress.find_last_of("\\");
	return fileAddress.substr(0,indexlastSlash);
	
}

