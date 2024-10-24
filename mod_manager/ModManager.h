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

	bool loadFile(std::string filepath);
	bool saveMod();

	void editModSettings();
	void addObserver(ModObserver* observer) { _observers.insert(observer); };
	void removeObserver(ModObserver* observer) { _observers.erase(observer); }

	void alertObservers();

	static ModManager& getInstance() {
		if (_singleton == nullptr) {
			_singleton = new ModManager();
		}

		return *_singleton;
	};

	Directory& getRootDir() const {
		if (_rootDir == nullptr) {
			throw std::runtime_error("Root directory is uninitialised.");
		}

		return *_rootDir;
	};

   private:
	static ModManager* _singleton;

	ModManager() = default;

	Mod _currentMod;

	// fs::path _contentsDirPath;
	Directory* _rootDir = nullptr;

	std::string _currentContentsDirectory;

	std::set<ModObserver*> _observers;
};
