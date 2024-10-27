#include "SD9Override.h"

#include <utility>
#include "audio/SD9File.h"
#include "mod_manager/Mod.h"

using std::string;

SD9Override::SD9Override(fs::path in, SD9Info info)
    : Override(std::move(in)), _info(info) {};

std::string SD9Override::serialiseData() {
    std::array<char, SD9_HEADER_SIZE> data{};

    memcpy(data.data(), &_info, SD9_HEADER_SIZE);

    std::stringstream ss;
    ss << _replacement_file << OVERRIDE_SEP_CHAR
       << std::string(data.data(), SD9_HEADER_SIZE);

    return ss.str();
};
