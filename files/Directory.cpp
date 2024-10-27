#include "Directory.h"

#include "utils.h"

Directory::Directory(fs::path path) {
    this->_path = path;
    this->_files = {};
    this->_subdirs = {};

    if (!fs::exists(path)) {
        throw std::invalid_argument("Bad path used: " + path.string());
    }

    this->buildDirectory();
}

void Directory::buildDirectory() {
    // Check all files in current working directory
    for (const auto& entry : fs::directory_iterator(this->getPath())) {
        // If its a file, add the filepath to the list of filenames
        if (fs::is_regular_file(entry)) {
            this->addFile(entry.path());
        }

        // For each directory, add it to the list and recursively check it
        else if (fs::is_directory(entry)) {
            Directory child = Directory(entry.path());
            if (child.isEmptyRecursive(true)) {
                continue;
            }

            this->addSubdir(std::move(child));
        }
    }
}

void Directory::addFile(const fs::path& path) { this->_files.push_back(path); }

bool Directory::isEmptyRecursive(bool check_file_extensions) const {
    if (check_file_extensions) {
        for (const fs::path& file : this->_files) {
            if (Utils::File::IsSupported(file)) {
                return false;
            }
        }
    } else {
        if (_files.size() > 0) {
            return false;
        }
    }

    for (const Directory& subdir : _subdirs) {
        if (!subdir.isEmptyRecursive(check_file_extensions)) {
            return false;
        }
    }

    return true;
};
