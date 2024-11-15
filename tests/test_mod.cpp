#include <doctest/doctest.h>
#include "mod_manager/Mod.h"

TEST_CASE("Mod Tests") {
    SUBCASE("Mod Deserialisation") {
        std::string base_mod_string =
            "mod_name pinky crush\n"
            "game_version 31\n"
            "mod_version_major 1\n"
            "mod_version_minor 0\n"
            "data_root_dir /home/lucas/Downloads\n"
            "BEGIN OVERRIDES\n";

        std::string data_test_1 = "SD9|\n";
        std::string data_test_2 = "SD9|data/test.sd9|music/music.wav\n";

        /*
std::string data_test_3 =
    "SD9|data/test.sd9|"
    "music.wav|12341234123412341234123412341234\n";
                */

        std::stringstream ss;

        ss = std::stringstream(base_mod_string + data_test_1);
        CHECK_THROWS(Mod::deserialise(ss));

        ss = std::stringstream(base_mod_string + data_test_2);
        CHECK_THROWS(Mod::deserialise(ss));

        /*
ss = std::stringstream(base_mod_string + data_test_3);
CHECK_NOTHROW(Mod::deserialise(ss));
        */
    }
}
