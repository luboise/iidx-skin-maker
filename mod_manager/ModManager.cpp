#include "ModManager.h"

#include <wx/msgdlg.h>

#include <stdexcept>

#include "ModObserver.h"
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
		this->alertObservers();
	} catch (std::exception& e) {
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

	changeContentsDirectory(new_mod.root_dir);

	// TODO: Add proper validation before overwriting
	bool valid = true;
	if (valid) {
		_currentMod = std::move(new_mod);
		this->alertObservers();
	}
};

void ModManager::alertObservers() {
	for (auto* observer : _observers) observer->onModChanged(_currentMod);
}

bool ModManager::saveMod() {
	// TODO: Implement serialisation of mod
	return true;
}

bool ModManager::loadFile(std::string mod) {
	// TODO: Implement deserialisation of mod
	return true;
}

void ModManager::editModSettings() {
	auto Popup = ModSettingsPopup(_currentMod);

	alertObservers();
};
