#include "library.h"
#include <iostream>
int main()
{
	std::string test;
	std::cin >> test;
	std::cout << extractFilePath(test);
	return 0;
}