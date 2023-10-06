#pragma once

#include <string>

#include "Directory.h"

#define SD9_HEADER_SIZE 32

class FileHandler {
   public:
	static Directory* scanDirectory(const fs::path& baseDir);

   public:
	static char* readSD9File(fs::path filePath, size_t& dataSize);

   private:
	static void buildTreeRecursive(Directory* currentDir);
};
