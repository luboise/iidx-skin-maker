#pragma once

#include <filesystem>
#include <list>
#include <vector>

namespace fs = std::filesystem;
using std::string;
using std::vector;

class Directory {
   public:
	Directory(fs::path path);

	string getName() const;
	fs::path getPath() const;

	void addFile(const fs::path& path);
	void addSubdir(Directory* path);

	std::list<fs::path> getFiles() const;
	std::list<Directory*> getDirs() const;

   private:
	fs::path path;
	std::list<fs::path> files;
	std::list<Directory*> subdirs;
};