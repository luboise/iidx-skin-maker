#pragma once

#include <map>
#include <memory>
#include "Overrides/Override.h"

struct Mod {
    std::string name = "My Mod";

    int32_t game_version = 31;

    int32_t version_major = 1;
    int32_t version_minor = 0;

    fs::path root_dir = "";

    std::map<fs::path, std::unique_ptr<Override>> overrides;

    bool hasOverride(const fs::path& path) {
        return this->overrides.find(path) != overrides.end();
    }

    void setOverride(const fs::path& path,
                     std::unique_ptr<Override>(override)) {
        if (overrides.contains(path)) {
            overrides.erase(path);
        }

        overrides[path] = std::move(override);
    };

    Override* getOverride(const fs::path& path) {
        auto found = overrides.find(path);
        if (found == overrides.end()) {
            return nullptr;
        }

        return found->second.get();
    }

    [[nodiscard]] std::string serialise() const;
    static Mod deserialise(const std::string&);
};
