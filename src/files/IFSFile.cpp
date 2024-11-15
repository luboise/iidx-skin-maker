#include "IFSFile.h"

#include "files/FileHandler.h"

void parseIFSFile(const fs::path& path) {
    auto chars = FileHandler::Read(path);
};
