#include "FileHandler.h"

#include <fstream>
#include <iostream>
using std::ifstream;

#define WAV_SUBCHUNK_VALUE_OFFSET 16

Directory* FileHandler::scanDirectory(const fs::path& baseDir) {
	auto initialPath = fs::path(baseDir);
	if (!fs::exists(initialPath)) {
		throw std::invalid_argument("Bad directory used: " +
									initialPath.string());
	}

	Directory* tree = new Directory(initialPath);
	buildTreeRecursive(tree);

	return tree;
}

/*
char* FileHandler::readSD9File(fs::path filePath, size_t& dataSize) {
	ifstream ifs(filePath, std::ios::binary);

	if (!ifs.is_open()) {
		return nullptr;
	}

	ifs.seekg(0, ifs.end);
	const size_t parseAmount = ifs.tellg() - SD9_HEADER_SIZE;
	ifs.seekg(SD9_HEADER_SIZE, ifs.beg);

	auto wavData = new char[parseAmount];
	ifs.read(wavData, parseAmount);

	dataSize = parseAmount;
	return wavData;
};
*/

void FileHandler::buildTreeRecursive(Directory* currentDir) {
	// Check everything in the current directory
	for (const auto& entry : fs::directory_iterator(currentDir->getPath())) {
		// For each directory, add it to the list and recursively check it
		if (fs::is_directory(entry)) {
			Directory* newDir = new Directory(entry.path());
			currentDir->addSubdir(newDir);
			buildTreeRecursive(newDir);
		}
		// If its a file, add the filepath to the list of filenames
		else if (fs::is_regular_file(entry)) {
			currentDir->addFile(entry.path());
		}
	}
}
