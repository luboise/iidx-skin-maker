#include "SD9File.h"

#include "files/FileHandler.h"

#include <fstream>

#include "SoundFile.h"

using std::ifstream;

void SD9File::initialiseFrom(std::istream& ifs) {
    // Get size of file
    ifs.seekg(0, std::ios::end);
    const int64_t size = ifs.tellg();

    // Read SD9 header
    ifs.seekg(0, std::ios::beg);

    std::array<char, SD9_HEADER_SIZE> sd9_header{};
    ifs.read(sd9_header.data(), sizeof(SD9Info));
    memcpy(&_sd9Header, sd9_header.data(), sizeof(SD9Info));

    std::vector<char> data(size - SD9_HEADER_SIZE + 1);
    ifs.read(data.data(), size);

    SoundFile::parseAudioData(data.data(), size);
};

/*
SD9File::SD9File(const char* filename) {
    ifstream ifs(filename);

    if (!ifs.is_open()) {
        std::stringstream ss;
        ss << "Input file stream could not be opened for " << filename;

        throw std::runtime_error(ss.str());
    }

    // Get size of file
    ifs.seekg(0, std::ios::end);
    const int64_t size = ifs.tellg();

    // Allocate memory for the data excluding the SD9 header
    char* data = new char[size - SD9_HEADER_SIZE + 1];

    // Read SD9 header
    ifs.seekg(size, std::ios::beg);
    ifs.read(static_cast<char*>(&_sd9Header), SD9_HEADER_SIZE);

    ifs.read(data, size);

    _soundFile = new SoundFile(data);
}
*/
