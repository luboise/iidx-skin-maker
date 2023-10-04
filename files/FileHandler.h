#include <string>

#include "Directory.h"

class FileHandler {
   public:
	static Directory* scanDirectory(const fs::path& baseDir);

   private:
	static void buildTreeRecursive(Directory* currentDir);
};
