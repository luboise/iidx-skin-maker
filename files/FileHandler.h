#include <string>

#include "Directory.h"





class FileHandler {
public:
	static Directory* getFileTree(string& baseDir);
private:
	static void buildTreeRecursive(Directory* currentDir);
};




