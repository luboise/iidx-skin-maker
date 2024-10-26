#include <doctest/doctest.h>
#include "mod_manager/Mod.h"

TEST_CASE("Mod Tests") {
    SUBCASE("Mod Deserialisation") {
        std::string mod_string =
            "mod_name vagina pinky crush\n"
            "game_version 31\n"
            "mod_version_major 1\n"
            "mod_version_minor 0\n"
            "data_root_dir /home/lucas/Downloads\n"
            "BEGIN OVERRIDES\n"
            "SD9|\n";

        CHECK_THROWS(Mod::deserialise(mod_string));
    }
}
