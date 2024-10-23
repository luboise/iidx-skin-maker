#pragma once

#include <filesystem>
#include <list>

class Directory {
   public:
	Directory(fs::path path);

	void addFile(const fs::path& path);
	void addSubdir(Directory&& dir) { this->_subdirs.push_back(dir); }

	// Getters
	std::string getName() const { return this->_path.filename().string(); };
	const fs::path getPath() const { return this->_path; };
	const std::list<fs::path>& getFiles() const { return _files; };
	const std::list<Directory>& getDirs() const { return _subdirs; };

   private:
	void buildDirectory();

	fs::path _path;
	std::list<fs::path> _files;
	std::list<Directory> _subdirs;
};
