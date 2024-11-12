#include "FileHandler.h"
#include "audio/AudioHandler.h"
#include "utils.h"

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

size_t FileHandler::GetFileSize(const fs::path& path) {
    std::ifstream ifs(path, std::ios::binary);

    if (!ifs.is_open()) {
        std::cerr << "Unable to read file " << path << ". Returning \"\""
                  << std::endl;
        return 0;
    }

    ifs.seekg(0, std::ios::end);
    const size_t file_size = ifs.tellg();

    ifs.close();

    return file_size;
};

void FileHandler::Preview(const fs::path& path) {
    if (!fs::exists(path) || !fs::is_regular_file(path)) {
        std::cerr
            << "File to be previewed does not exist or is otherwise invalid: "
            << path << path << std::endl;
        return;
    }

    if (!Utils::File::IsSupported(path)) {
        std::cerr << "Unsupported file is not able to be previewed: " << path
                  << std::endl;
        return;
    }

    fs::path extension = path.extension();

    if (extension == SUPPORTED_FILE_EXTENSIONS::SD9) {
        try {
            AudioHandler::PlaySD9(path);
        } catch (std::exception& e) {
            Utils::Dialog::Error("Unable to preview SD9 file " + path.string() +
                                 ".\nError: " + e.what());
        }

    }

    else {
        std::cerr << "Unable to preview extension " << extension << std::endl;
        return;
    }
};
