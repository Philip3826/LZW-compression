#pragma once
#include <fstream>
#include <filesystem>
#include <string>

std::ofstream& compress(std::ofstream& file, std::string& fileAddress);
std::string extractFilePath(std::string& fileAddress);

