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

Mod Mod::deserialise(std::istream& stream) {
    Mod mod{};

    bool processing_overrides = false;

    std::string line;
    while (std::getline(stream, line)) {
        if (processing_overrides) {
            vector<std::string> tokens =
                Utils::String::Split(line, OVERRIDE_SEP_CHAR);

            if (tokens.size() != 4) {
                std::stringstream ss;
                ss << "Line does not have 4 tokens: \n"
                   << line << "\n\nSkipping this override." << std::endl;

                throw std::invalid_argument(ss.str());
            }

            Override* override = nullptr;

            if (tokens[0] == SD9_TYPE) {
                override =
                    new SD9Override(tokens[1], SD9Info::from(tokens[3].data()));

                if (!fs::exists(tokens[2])) {
                    std::cerr << "Unable to process replacement path for "
                                 "SD9Override: "
                              << tokens[2] << "\nSkipping this override."
                              << std::endl;
                    delete override;

                    continue;
                }

                (dynamic_cast<SD9Override*>(override))
                    ->setReplacementFilepath(tokens[2]);
            } else {
                throw std::invalid_argument(
                    "Invalid token type found for type of override: " +
                    tokens[0]);
            }

            if (override == nullptr) {
                throw std::runtime_error("Unable to pass override line:\n" +
                                         line);
            }

            mod.setOverride(tokens[1], std::move(override));

            continue;
        } else if (line == OVERRIDES_BEGIN) {
            processing_overrides = true;
            continue;
        }

        std::string data = get_line_data(line);
        if (line.starts_with(NAME)) {
            mod.name = data;

            continue;
        } else if (line.starts_with(DATA_DIRECTORY)) {
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
        } else {
            throw std::runtime_error("Invalid line found:\n" + line);
        }
    }

    return mod;
};
