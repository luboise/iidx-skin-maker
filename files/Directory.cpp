#include "Directory.h"

Directory::Directory(fs::path path) {
	this->_path = path;
	this->_files = {};
	this->_subdirs = {};

	if (!fs::exists(path)) {
		throw std::invalid_argument("Bad path used: " + path.string());
	}

	this->buildDirectory();
}

void Directory::buildDirectory() {
	// Check everything in the current directory
	for (const auto& entry : fs::directory_iterator(this->getPath())) {
		// For each directory, add it to the list and recursively check it
		if (fs::is_directory(entry)) {
			Directory child = Directory(entry.path());
			this->addSubdir(std::move(child));
		}

		// If its a file, add the filepath to the list of filenames
		else if (fs::is_regular_file(entry)) {
			this->addFile(entry.path());
		}
	}
}

void Directory::addFile(const fs::path& path) { this->_files.push_back(path); }
