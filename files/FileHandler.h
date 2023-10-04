#include <string>

#include "Directory.h"





class FileHandler {
public:
	static Directory* getFileTree(fs::path& baseDir);
private:
	static void buildTreeRecursive(Directory* currentDir);
};




