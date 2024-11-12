#pragma once

#include <set>
#include <stdexcept>

#include "Mod.h"
#include "files/Directory.h"

class ModObserver;

class ModManager {
   public:
    bool changeContentsDirectory(const fs::path&);

    void newMod();

    bool loadMod(const fs::path&);

    static bool saveMod(const Mod&, const fs::path&);
    bool saveMod() { return saveMod(_currentMod, _modLocation); };

    void editModSettings();

    void selectPath(fs::path& path);

    fs::path getSelectedPath(bool get_full_path = false) {
        return get_full_path ? this->getRootPath() / _selectedPath
                             : _selectedPath;
    };

    Override* getOverride(const fs::path& path) {
        return _currentMod.getOverride(path);
    }

    [[nodiscard]] Override* getMostRecentOverride() const {
        return _mostRecentOverride;
    };

    void addOverride(Override* override) {
        fs::path in_path = override->proximatePath();
        _currentMod.setOverride(in_path, override);
        _mostRecentOverride = override;

        alertObservers(ALERT_TYPE::OVERRIDE_UPDATED);
    }

    void removeOverride(Override* override) {
        _currentMod.removeOverride(override);
        _mostRecentOverride = nullptr;

        alertObservers(ALERT_TYPE::OVERRIDE_UPDATED);
    }

    void addObserver(ModObserver* observer) { _observers.insert(observer); };
    void removeObserver(ModObserver* observer) { _observers.erase(observer); }
    void alertObservers(ALERT_TYPE type);

    [[nodiscard]] bool exportMod(const fs::path&) const;

    static void Export(const Mod&, const fs::path& out_parent_folder);

    static ModManager& getInstance() {
        if (_singleton == nullptr) {
            auto* mod_manager{new ModManager()};
            _singleton = mod_manager;
        }

        return *_singleton;
    };

    [[nodiscard]] Directory& getRootDir() const {
        if (_rootDir == nullptr) {
            throw std::runtime_error("Root directory is uninitialised.");
        }

        return *_rootDir;
    };

    [[nodiscard]] fs::path getRootPath() const {
        if (_rootDir == nullptr) {
            throw std::runtime_error("Root directory is uninitialised.");
        }

        return _rootDir->getPath();
    };

   private:
    static ModManager* _singleton;

    ModManager() = default;

    fs::path _modLocation = "";
    Mod _currentMod{};

    fs::path _selectedPath = "";

    // fs::path _contentsDirPath;
    Directory* _rootDir = nullptr;

    std::string _currentContentsDirectory;

    std::set<ModObserver*> _observers;

    Override* _mostRecentOverride{nullptr};
};
