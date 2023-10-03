#include "FileHandler.h"

#include <iostream>

namespace fs = std::filesystem;

void FileHandler::getFileTree(string& baseDir)
{
	for (const auto& entry : fs::recursive_directory_iterator(baseDir)) {
		if (fs::is_regular_file(entry)) {
			std::cout << entry.path() << std::endl;
		}
	}
}