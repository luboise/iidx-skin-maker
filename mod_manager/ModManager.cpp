#include "ModManager.h"

#include "ModObserver.h"

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

	_currentMod = Mod();

	this->alertObservers();
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
