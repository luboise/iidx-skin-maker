#include "Directory.h"

Directory::Directory(fs::path path) {
	this->path = path;
	this->files;
	this->subdirs;
}

string Directory::getName() const { return this->path.filename().string(); }

fs::path Directory::getPath() const { return this->path; }

void Directory::addFile(const fs::path& path) { this->files.push_back(path); }

void Directory::addSubdir(Directory* dir) {
	if (dir == nullptr) {
		throw std::logic_error("Received null directory.");
	}
	this->subdirs.push_back(dir);
}

std::list<fs::path> Directory::getFiles() const { return this->files; }
std::list<Directory*> Directory::getDirs() const { return this->subdirs; }
