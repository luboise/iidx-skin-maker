#include "Mod.h"

#include <filesystem>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "utils.h"

#include "Overrides/SD9Override.h"

constexpr char SPLITTER_CHAR[] = " ";

constexpr char NAME[] = "mod_name";
constexpr char GAME_VERSION[] = "game_version";
constexpr char MOD_VERSION_MAJOR[] = "mod_version_major";
constexpr char MOD_VERSION_MINOR[] = "mod_version_minor";
constexpr char DATA_DIRECTORY[] = "data_root_dir";

constexpr char OVERRIDES_BEGIN[] = "BEGIN OVERRIDES";

std::string get_line_data(const std::string& line) {
    size_t index = line.find(SPLITTER_CHAR);
    if (index == -1) {
        return "";
    }

    return line.substr(index + 1);
};

std::string Mod::serialise() const {
    std::stringstream ss;
    ss << NAME << SPLITTER_CHAR << this->name << "\n";
    ss << GAME_VERSION << SPLITTER_CHAR << this->game_version << "\n";
    ss << MOD_VERSION_MAJOR << SPLITTER_CHAR << this->version_major << "\n";
    ss << MOD_VERSION_MINOR << SPLITTER_CHAR << this->version_minor << "\n";
    ss << DATA_DIRECTORY << SPLITTER_CHAR << this->root_dir.string() << "\n";

    ss << OVERRIDES_BEGIN << "\n";

    for (const auto& pair : this->overrides) {
        ss << pair.second->serialise();
        ss << "\n";
    }

    return ss.str();
}

Mod Mod::deserialise(const std::string& serialised_data) {
    std::stringstream ss(serialised_data);

    Mod mod{};

    bool processing_overrides = false;

    std::string line;
    while (std::getline(ss, line)) {
        if (processing_overrides) {
            vector<std::string> tokens =
                Utils::String::Split(line, OVERRIDE_SEP_CHAR);

            if (tokens.size() != 4) {
                std::cerr << "Line does not have 4 tokens: \n"
                          << line << "\n\nSkipping this override." << std::endl;
                continue;
            }

            if (tokens[0] == SD9Override::getType()) {
                auto override = SD9Override(tokens[1], tokens[2],
                                            SD9Info::from(tokens[3].data()));
            } else {
                throw std::invalid_argument(
                    "Invalid token type found for type of override: " +
                    tokens[0]);
            }

            continue;
        } else if (line == OVERRIDES_BEGIN) {
            processing_overrides = true;
            continue;
        }

        std::string data = get_line_data(line);
        if (line.starts_with(NAME)) {
            mod.name = data;

            continue;
        }
        if (line.starts_with(DATA_DIRECTORY)) {
            auto path = fs::path(data);

            if (!fs::exists(path)) {
                std::cerr << "Unable to parse data dir as path: " << data
                          << ". Ignoring.";
            } else {
                mod.root_dir = path;
            }

            continue;
        }

        else if (line.starts_with(GAME_VERSION)) {
            try {
                mod.game_version = std::stoi(data);
            } catch (std::invalid_argument& e) {
                std::cerr << "Unable to parse game version: " << data
                          << ". Ignoring." << std::endl;
            }
        } else if (line.starts_with(MOD_VERSION_MAJOR)) {
            try {
                mod.version_major = std::stoi(data);
            } catch (std::invalid_argument& e) {
                std::cerr << "Unable to parse mod version major: " << data
                          << ". Ignoring." << std::endl;
            }
        } else if (line.starts_with(MOD_VERSION_MINOR)) {
            try {
                mod.version_minor = std::stoi(data);
            } catch (std::invalid_argument& e) {
                std::cerr << "Unable to parse mod version minor: " << data
                          << ". Ignoring." << std::endl;
            }
        }
    }

    return mod;
};
