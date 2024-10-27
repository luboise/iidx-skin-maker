#include "ModManager.h"

#include <wx/msgdlg.h>
#include <wx/popupwin.h>

#include "ModObserver.h"
#include "files/FileHandler.h"
#include "gui/Popups/ModSettingsPopup.h"
#include "utils.h"

ModManager* ModManager::_singleton = nullptr;

bool ModManager::changeContentsDirectory(const fs::path& newDir) {
    try {
        auto* dir = new Directory(newDir);
        if (_rootDir != nullptr) {
            delete _rootDir;
            _rootDir = nullptr;
        }
        _rootDir = dir;

        _currentMod.root_dir = newDir;

        this->alertObservers(ALERT_TYPE::MOD_CHANGED);
    } catch (std::exception& e) {
        std::cerr << "Unable to update contents directory. Error: " << e.what()
                  << std::endl;
        return false;
    }

    return true;
}

void ModManager::newMod() {
    /*
    if (_currentMod != nullptr) {
            delete _currentMod;
            _currentMod = nullptr;
    }
    */

    // fs::path new_root = Utils::directoryPopup("Select your data folder.");

    auto new_mod = Mod();

    auto* dialog = new ModSettingsPopup(new_mod);
    dialog->ShowModal();

    if (!fs::exists(new_mod.root_dir)) {
        wxMessageBox(
            "Unable to create new mod from invalid path. Continuing to use the "
            "old path.");
        return;
    }

    fs::path save_path = Utils::saveFilePopup(MOD_FILE_EXTENSION);

    // TODO: Add proper validation before overwriting
    bool valid = true;
    if (valid) {
        _currentMod = std::move(new_mod);
        _modLocation = save_path;

        this->saveMod();

        changeContentsDirectory(_currentMod.root_dir);
    }
};

void ModManager::alertObservers(ALERT_TYPE type) {
    if (type == ALERT_TYPE::MOD_CHANGED) {
        for (auto* observer : _observers) {
            observer->onModChanged(_currentMod);
        }
    } else if (type == ALERT_TYPE::SELECTED_PATH_CHANGED) {
        for (auto* observer : _observers) {
            observer->onSelectedPathChanged(PathChangedData{
                _selectedPath, ModManager::getOverride(_selectedPath)});
        }
    } else if (type == ALERT_TYPE::OVERRIDE_UPDATED) {
        for (auto* observer : _observers) {
            // TODO: Implement this
            // observer->onOverrideUpdated(_selectedPath);
        }
    } else {
        std::cerr << "Invalid alert type encountered in alertObservers: "
                  << static_cast<int>(type)
                  << ".\nIgnoring this and continuing with execution."
                  << std::endl;
    }
}

bool ModManager::saveMod(const Mod& mod, const fs::path& location) {
    fs::path backup_path = FileHandler::Backup(location);
    FileHandler::Write(mod.serialise(), location);

    // fs::path path = Utils::saveFilePopup("2dxmod");

    if (fs::exists(location)) {
        // if (!Utils::booleanPopup("Would you like to overwrite this file?")) {
        // return false;
        //}
    }

    FileHandler::Delete(backup_path);
    return true;
}

bool ModManager::loadMod(const fs::path& mod_path) {
    std::string data = FileHandler::Read(mod_path);

    if (data == "") {
        std::cerr << "Invalid file read for mod path " << mod_path
                  << ". Unable to load mod." << std::endl;
        return false;
    }

    Mod m = Mod::deserialise(data);

    _currentMod = std::move(m);
    _modLocation = mod_path;

    changeContentsDirectory(_currentMod.root_dir);

    alertObservers(ALERT_TYPE::MOD_CHANGED);

    return true;
}

void ModManager::editModSettings() {
    auto Popup = ModSettingsPopup(_currentMod);

    alertObservers(ALERT_TYPE::MOD_CHANGED);
};

void ModManager::selectPath(fs::path& path) {
    if (!fs::exists(path)) {
        wxMessageBox(
            std::string("File ") + path.string() +
            std::string(
                " does not exist. Please refresh your contents folder."));
        return;
    }

    _selectedPath = path;
    alertObservers(ALERT_TYPE::SELECTED_PATH_CHANGED);

    /*
try {
    ifstream ifs(path);

    SD9File* song = new SD9File(ifs);
    AudioHandler::PlaySD9(*song);

    // AudioHandler::PlayPCM(bm);
} catch (std::runtime_error e) {
    std::stringstream ss;
    ss << "ERROR: Unable to play file from path \"" << path
       << "\". \nError message: " << e.what() << ")";
    wxMessageBox(ss.str().c_str());

} catch (std::exception e) {
    wxMessageBox("An unhandled exception has occurred.");
}
    */
};
