#include "FileHandler.h"

#include <iostream>

Directory FileHandler::getFileTree(string& baseDir)
{
	auto initialPath = fs::path(baseDir);
	if (!fs::exists(initialPath))
	{
		throw std::invalid_argument("Bad directory used: " + initialPath.string());
	}

	Directory fileTree(initialPath);
	makeTreeRecursive(fileTree);
};

void FileHandler::makeTreeRecursive(Directory& currentDir)
{
	// Check everything in the current directory
	for (const auto& entry : fs::directory_iterator(currentDir.getPath())) {
		// For each directory, add it to the list and recursively check it
		if (fs::is_directory(entry)) {
			Directory newDir(entry.path());
			currentDir.addSubdir(newDir);
			makeTreeRecursive(newDir);
		}
		// If its a file, add the filepath to the list of filenames
		else if (fs::is_regular_file(entry)) {
			currentDir.addFile(entry.path());
		}
		std::cout << entry.path() << std::endl;
	}
}


