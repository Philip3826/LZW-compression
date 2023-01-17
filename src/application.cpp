#include "library.h"
#include "Compressor.h"
#include <iostream>
int main()
{
	std::filesystem::path test;
	std::cin >> test;
	Compressor comp;
	std::vector<uint16_t> result = comp.compress(test);
	std::string decompress = comp.decompress(test, result);
	std::ofstream fs("C:\\Users\\phili\\Desktop\\dec.txt", std::ios::binary);
	if (fs.is_open())
	{
		for (std::vector<uint16_t>::iterator it = result.begin(); it != result.end(); it++)
		{
			fs.write(reinterpret_cast<const char*>(&*it), sizeof(uint16_t));
		}
	}
	std::cout << decompress;
	return 0;
}