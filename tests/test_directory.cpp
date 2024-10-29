#include <doctest/doctest.h>

TEST_CASE("Directory Tests") {
    SUBCASE("Test getProximated()") {
        fs::path root_path = "/home/data/";
        fs::path filepath = "/home/data/extra/1/x";

        CHECK_EQ(filepath.lexically_proximate(root_path), "extra/1/x");
    }
}
