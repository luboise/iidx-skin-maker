#pragma once

#include <wx/debug.h>
#include <array>

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
    int fluff4 = 0x0;

    int unique_index = 0x0;
};

class SD9File {
   public:
    // SD9File(const char* filename);
    explicit SD9File(std::ifstream&);

    const SoundFile* getSoundFile() const { return _soundFile; }

    ~SD9File();

   private:
    SD9Info _sd9Header{};
    SoundFile* _soundFile;
};
