#pragma once

#include <iostream>
#include <optional>
#include "audio/SD9File.h"

#include "mixins/Serialisable.h"
#include "mod_manager/Mod.h"

constexpr auto SD9_TYPE = "SD9";

struct SD9InfoOverride {
    std::optional<std::array<char, 4>> header{std::nullopt};

    std::optional<uint32_t> header_size{SD9_HEADER_SIZE};  // 4-7

    std::optional<uint8_t> volume{std::nullopt};

    std::optional<int8_t> fluff3{std::nullopt};

    std::optional<uint32_t> loop_start_byte_offset{std::nullopt};
    std::optional<uint32_t> loop_end_byte_offset{std::nullopt};

    std::optional<int8_t> loop_enabled{std::nullopt};
    std::optional<uint8_t> fluff4{std::nullopt};

    std::optional<uint16_t> unique_index{std::nullopt};
};

struct SD9Replacement {
    fs::path path = "";
    SoundFile* new_audio = nullptr;
    SD9Info base_info;
    SD9InfoOverride override_info{};
};

class SD9Override : public Override {
   public:
    explicit SD9Override(fs::path in);
    // SD9Override(fs::path in, SD9Info info);
    SD9Override(fs::path in, SD9InfoOverride overrideInfo);

    std::string serialiseData() override;
    std::string getType() override { return SD9_TYPE; };

    [[nodiscard]] fs::path getReplacementFilepath() const {
        return _replacementData.path;
    };

    void setReplacementFilepath(const fs::path& path) {
        if (!fs::exists(path)) {
            std::cerr << "Replacement file " + path.string() +
                             " does not exist on filesystem, or is otherwise "
                             "invalid.";
            return;
        }

        try {
            auto* new_replacement{new SoundFile(path)};

            delete _replacementData.new_audio;

            _replacementData.path = path;
            _replacementData.new_audio = new_replacement;
        } catch (std::exception& e) {
            std::cerr << "Unable to set replacement file.\nError details: "
                      << e.what() << std::endl;
        }
    }

    void process(const ProcessData& process_data) override;

    [[nodiscard]] const SD9Replacement& getOverrideData() const {
        return _replacementData;
    }

    [[nodiscard]] SD9InfoOverride& getOverrideList() {
        return _replacementData.override_info;
    }

   private:
    [[nodiscard]] SD9Info getSD9InfoOverride() const;
    SD9Replacement _replacementData{};
};
