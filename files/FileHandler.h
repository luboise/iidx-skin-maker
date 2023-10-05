#include <string>

#include "Directory.h"

class FileHandler {
   public:
	static Directory* scanDirectory(const fs::path& baseDir);

   public:
	static char* readSD9File(fs::path filePath, size_t& dataSize);

   private:
	static void buildTreeRecursive(Directory* currentDir);
};
