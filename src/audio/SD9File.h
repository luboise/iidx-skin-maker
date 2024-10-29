#pragma once

#include "files/FileHandler.h"

#include <array>
#include <sstream>

#include "SoundFile.h"

constexpr auto SD9_HEADER_SIZE = 32;
constexpr auto FLUFF2_VALUE = 0x4000;

// SD9 layout from
// https://github.com/TheFooestBar/SD9Tool/blob/master/sd9tool.py
struct SD9Info {
    std::array<char, 4> header = {'S', 'D', '9', '\0'};

    uint32_t header_size = SD9_HEADER_SIZE;
    uint32_t audio_size = 0;

    // Gathered from existing file
    uint32_t fluff1 = 0x0;

    // Must always be this value
    uint16_t fluff2 = FLUFF2_VALUE;

    // Volume from 0-255 (0 is loudest)
    uint8_t volume = 0x0;

    // ?
    uint8_t fluff3 = 0x0;

    // Loop vars in samples
    uint32_t loop_start = 0x0000;
    uint32_t loop_end = 0x0000;

    uint8_t loop_enabled = 0x0;

    // ?
    uint8_t fluff4 = 0x0;

    uint16_t unique_index = 0x0;

    static SD9Info from(char* data) {
        SD9Info info;
        memcpy(&info, data, sizeof(SD9Info));
        return info;
    };
};

class SD9File : public SoundFile {
   public:
    // SD9File(const char* filename);
    explicit SD9File(const fs::path& path) {
        std::string data = FileHandler::Read(path);
        auto ss = std::stringstream(data);
        this->initialiseFrom(ss);
    };
    explicit SD9File(std::istream& in) { this->initialiseFrom(in); };

    SD9File(const SD9File& other) = default;
    SD9File& operator=(const SD9File& other) = default;

    SD9File(SD9File&& other) = default;
    SD9File& operator=(SD9File&& other) = default;

    [[nodiscard]] SD9Info getSD9Info() const { return _sd9Header; }

   private:
    void initialiseFrom(std::istream&);

    SD9Info _sd9Header{};
};
