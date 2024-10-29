#include "SD9Override.h"

#include <filesystem>
#include <fstream>
#include <utility>
#include "audio/SD9File.h"
#include "mod_manager/Mod.h"
#include "mod_manager/ModManager.h"

using std::string;

SD9Override::SD9Override(fs::path in, SD9Info info) : Override(std::move(in)) {
    _replacementData.info = info;
}

SD9Override::SD9Override(fs::path in) : Override(std::move(in)) {
    fs::path root_path = ModManager::getInstance().getRootPath();
    auto sd9 = SD9File(root_path / proximatePath());

    _replacementData.info = sd9.getSD9Info();
}

std::string SD9Override::serialiseData() {
    std::array<char, SD9_HEADER_SIZE> sd9_data{};

    memcpy(sd9_data.data(), &_replacementData.info, SD9_HEADER_SIZE);

    std::stringstream ss;
    ss << _replacementData.path.string() << OVERRIDE_SEP_CHAR
       << std::string(sd9_data.data(), SD9_HEADER_SIZE);

    return ss.str();
};

void SD9Override::process(const ProcessData& process_data) {
    if (!fs::exists(_replacementData.path)) {
        std::cerr << "Unable to process SD9Override: " << _replacementData.path
                  << std::endl;

        return;
    }

    fs::path out_fs_path = process_data.out_root / _proximatePath;

    fs::create_directories(out_fs_path.parent_path());

    _replacementData.audio->exportToFile(out_fs_path);

    std::string data = FileHandler::Read(out_fs_path);

    std::ofstream ofs(out_fs_path);

    ofs.write((char*)(&_replacementData.info), sizeof(_replacementData.info));
    ofs.write(data.data(), data.size());

    ofs.close();
}
