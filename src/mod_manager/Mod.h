#pragma once

#include <map>
#include <memory>
#include "Overrides/Override.h"

#define CURRENT_GAME_VERSION 31

struct Mod {
    std::string name = "My Mod";

    int32_t game_version = CURRENT_GAME_VERSION;

    int32_t version_major = 1;
    int32_t version_minor = 0;

    fs::path root_dir = "";

    std::map<fs::path, Override*> overrides;

    bool hasOverride(const fs::path& path) {
        return this->overrides.find(path) != overrides.end();
    }

    void setOverride(const fs::path& path, Override* override) {
        if (overrides.contains(path)) {
            overrides.erase(path);
        }

        overrides[path] = override;
    };

    void removeOverride(Override* override) {
        for (auto& pair : this->overrides) {
            if (pair.second == override) {
                delete pair.second;
                this->overrides.erase(pair.first);
                return;
            }
        }

        std::cerr << "WARNING: Attempted to remove override " << override
                  << " which does not belong to the mod." << std::endl;
    }

    Override* getOverride(const fs::path& path) {
        auto found = overrides.find(path);
        if (found == overrides.end()) {
            return nullptr;
        }

        return found->second;
    }

    [[nodiscard]] std::string serialise() const;
    static Mod deserialise(std::istream&);
};
