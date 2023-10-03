#include <filesystem>
#include <vector>
#include <list>

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
private:
	fs::path path;
	std::list<fs::path> files;
	std::list<Directory*> subdirs;
};