#include "FileHandler.h"

#include <filesystem>
#include <fstream>

fs::path FileHandler::Backup(const fs::path& from) {
    if (!fs::exists(from)) {
        std::cerr << "Filepath " << from << " could not be found." << std::endl;
        return "";
    }

    // Check if its a file
    if (!from.has_filename()) {
        std::cerr << "The specified file to backup " << from
                  << " is not a file that can be copied." << std::endl;
        return "";
    }

    fs::path to = from;
    to.replace_filename(from.filename().string() + "bak");

    fs::copy_file(from, to);

    return to;
}

bool FileHandler::Write(const std::string& data, const fs::path& filepath) {
    std::ofstream os(filepath);

    if (!os.is_open()) {
        std::cerr << "Unable to open file " << filepath << ".";
        return false;
    }

    os << data;
    os.close();

    return true;
};

bool FileHandler::Delete(const fs::path& file) {
    try {
        fs::remove(file);
        return true;
    } catch (std::exception& e) {
        std::cerr << "Unable to delete file. Error: " << e.what() << ". ";
        return false;
    }
}

std::string FileHandler::Read(const fs::path& filepath) {
    std::ifstream ifs(filepath);

    if (!ifs.is_open()) {
        std::cerr << "Unable to read file " << filepath << ". Returning \"\""
                  << std::endl;
        return "";
    }

    ifs.seekg(0, std::ios::end);
    const long file_size = ifs.tellg();

    ifs.seekg(0);

    std::string s(file_size, '\0');
    ifs.read(s.data(), file_size);

    ifs.close();

    return s;
};
