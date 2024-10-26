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

    bool saveMod(const Mod&, const fs::path&);
    bool saveMod() { return saveMod(_currentMod, _modLocation); };

    void editModSettings();

    void selectPath(fs::path& path);

    Override* getOverride(const fs::path& path) {
        return _currentMod.getOverride(path);
    }

    void addObserver(ModObserver* observer) { _observers.insert(observer); };
    void removeObserver(ModObserver* observer) { _observers.erase(observer); }
    void alertObservers();

    static ModManager& getInstance() {
        if (_singleton == nullptr) {
            _singleton = new ModManager();
        }

        return *_singleton;
    };

    [[nodiscard]] Directory& getRootDir() const {
        if (_rootDir == nullptr) {
            throw std::runtime_error("Root directory is uninitialised.");
        }

        return *_rootDir;
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
};
