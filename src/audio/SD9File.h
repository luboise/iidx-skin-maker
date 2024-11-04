#pragma once

#include "files/FileHandler.h"

#include <array>
#include <cstring>
#include <format>
#include <sstream>

#include "SoundFile.h"

constexpr auto SD9_HEADER_SIZE = 32;

// Size of a single sample in bytes
constexpr auto SD9_SAMPLE_SIZE = 4;

constexpr auto FLUFF2_VALUE = 0x4000;

// SD9 layout from
// https://github.com/TheFooestBar/SD9Tool/blob/master/sd9tool.py

struct SD9Info {
    struct {
        std::array<char, 4> header = {'S', 'D', '9', '\0'};  // 0-3

        uint32_t header_size = SD9_HEADER_SIZE;  // 4-7
        uint32_t audio_size = 0;                 // 8-11

        uint32_t fluff1 = 0x0;  // 12-15

        // Must always be this value
        uint16_t fluff2 = FLUFF2_VALUE;  // 16-17

        // Volume from 0-255 (0 is loudest)
        uint8_t volume = 0x0;  // 18

        // ?
        int8_t fluff3 = 0x0;  // 19

        // Loop vars in samples
        uint32_t loop_start_byte_offset = 0x0000;  // 20-23
        uint32_t loop_end_byte_offset = 0x0000;    // 24-27

        int8_t loop_enabled = 0x0;  // 28

        // ?
        uint8_t fluff4 = 0x0;  // 29

        uint16_t unique_index = 0x0;  // 30-31
    };

    static SD9Info from(void* data) {
        SD9Info info;
        memcpy(&info, data, sizeof(SD9Info));

        unsigned char* char_pointer = (unsigned char*)info.header.data();
        std::cout << "Outputting hex: \n";
        for (auto group = 0; group < SD9_HEADER_SIZE / 8; group++) {
            for (auto i = 0; i < 8; i++) {
                std::cout << std::format("{:#0x} ",
                                         *(char_pointer + 8 * group + i));
            }
            std::cout << std::endl;
        }

        return info;
    };

    [[nodiscard]] bool isLooping() const { return this->loop_enabled != 0; }
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
