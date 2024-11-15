#include <doctest/doctest.h>
#include "audio/SD9File.h"
#include "utils.h"

TEST_CASE("SD9Info Tests") {
    SUBCASE("Reading SD9Info from binary") {
        std::array<unsigned char, SD9_HEADER_SIZE> example_sd9 = {
            0x53, 0x44, 0x39, 0x00, 0x20, 0x00, 0x00, 0x00, 0xd0, 0x1f, 0x28,
            0x00, 0x31, 0x32, 0x02, 0x00, 0x40, 0x00, 0x14, 0x00, 0x84, 0x74,
            0x04, 0x00, 0xa8, 0x18, 0x28, 0x00, 0x01, 0x00, 0x13, 0x00

        };
        auto info{Utils::Struct::FromBinary<SD9Info>(example_sd9.data())};

        CHECK_EQ(info.loop_start_byte_offset, 291972);
        CHECK_EQ(info.loop_end_byte_offset, 2627752);
        CHECK_EQ(info.audio_size, 2629584);
    }
}
