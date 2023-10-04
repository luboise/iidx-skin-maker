#include <string>

#include "Directory.h"

class FileHandler {
   public:
	static Directory* scanDirectory(fs::path& baseDir);

   private:
	static void buildTreeRecursive(Directory* currentDir);
};
