#include "Directory.h"
#include <algorithm>

#include "utils.h"

Directory::Directory(const fs::path& path)
    : _path(path), _files({}), _subdirs({}) {
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
        if (std::ranges::any_of(_files.begin(), _files.end(),
                                [](const auto& file) {
                                    return Utils::File::IsSupported(file);
                                })) {
            return false;
        }
    } else {
        if (_files.size() > 0) {
            return false;
        }
    }

    // Check if all of the subdirs are empty
    return std::ranges::all_of(
        _subdirs.begin(), _subdirs.end(),
        [check_file_extensions](const auto& subdir) {
            return subdir.isEmptyRecursive(check_file_extensions);
        });
};
