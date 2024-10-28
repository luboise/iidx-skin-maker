#include "SD9Override.h"

#include <utility>
#include "audio/SD9File.h"
#include "mod_manager/Mod.h"

using std::string;

SD9Override::SD9Override(fs::path in, SD9Info info) : Override(std::move(in)) {
    _replacementData.info = info;
}

SD9Override::SD9Override(fs::path in) : Override(std::move(in)) {
    auto sd9 = SD9File(getInPath());
    _replacementData.info = sd9.getSD9Info();
}

std::string SD9Override::serialiseData() {
    std::array<char, SD9_HEADER_SIZE> data{};

    memcpy(data.data(), &_replacementData.info, SD9_HEADER_SIZE);

    std::stringstream ss;
    ss << _replacementData.path.string() << OVERRIDE_SEP_CHAR
       << std::string(data.data(), SD9_HEADER_SIZE);

    return ss.str();
};

void SD9Override::process(fs::path out_path) {
    if (!fs::exists(_replacementData.path)) {
        std::cerr << "Unable to process SD9Override: " << _replacementData.path
                  << std::endl;

        return;
    }

    _replacementData.audio->exportToFile(out_path / _in.filename());
}
