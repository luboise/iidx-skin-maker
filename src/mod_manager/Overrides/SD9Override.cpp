#include "SD9Override.h"

#include <filesystem>
#include <fstream>
#include <ios>
#include <utility>
#include "audio/SD9File.h"
#include "mod_manager/Mod.h"
#include "mod_manager/ModManager.h"

using std::string;

SD9Override::SD9Override(fs::path in, SD9InfoOverride overrideInfo)
    : SD9Override(std::move(in)) {
    _replacementData.override_info = overrideInfo;
}

SD9Override::SD9Override(fs::path in) : Override(std::move(in)) {
    fs::path root_path = ModManager::getInstance().getRootPath();
    auto sd9 = SD9File(root_path / proximatePath());

    _replacementData.base_info = sd9.getSD9Info();
}

std::string SD9Override::serialiseData() {
    constexpr auto BUFFER_SIZE = sizeof(SD9InfoOverride);

    std::array<char, BUFFER_SIZE> outputData{};

    memcpy(outputData.data(), &_replacementData.override_info,
           sizeof(typeof _replacementData.override_info));

    std::stringstream ss;
    ss << _replacementData.path.string() << OVERRIDE_SEP_CHAR
       << std::string(outputData.data(), BUFFER_SIZE);

    return ss.str();
};

void SD9Override::process(const ProcessData& process_data) {
    if (!fs::exists(_replacementData.path)) {
        std::cerr << "Unable to process SD9Override: " << _replacementData.path
                  << std::endl;

        return;
    }

    // Export the wav file
    fs::path out_fs_path = process_data.out_root / _proximatePath;
    fs::create_directories(out_fs_path.parent_path());
    _replacementData.new_audio->exportToFile(out_fs_path);

    // Read the wav file
    std::string data = FileHandler::Read(out_fs_path);
    std::ofstream ofs(out_fs_path);

    // Update the SD9 Override Info
    SD9Info final_info = this->getSD9InfoOverride();

    final_info.audio_size = FileHandler::GetFileSize(out_fs_path);

    // Write the SD9 header

    ofs.write(reinterpret_cast<char*>(&final_info),
              sizeof(_replacementData.base_info));
    ofs.write(data.data(), static_cast<std::streamsize>(data.size()));

    ofs.close();
}

SD9Info SD9Override::getSD9InfoOverride() const {
    SD9Info out_info = _replacementData.base_info;

#define COPY_IF_VAL(VAL)                                        \
    {                                                           \
        auto& value = this->_replacementData.override_info.VAL; \
        if (value.has_value()) {                                \
            out_info.VAL = value.value();                       \
        }                                                       \
    }

    COPY_IF_VAL(header);
    COPY_IF_VAL(header_size);
    COPY_IF_VAL(volume);
    COPY_IF_VAL(fluff3);
    COPY_IF_VAL(loop_start_byte_offset);
    COPY_IF_VAL(loop_end_byte_offset);
    COPY_IF_VAL(fluff4);
    COPY_IF_VAL(unique_index);

    return out_info;
};
