#pragma once

#include <set>

#include "Mod.h"
#include "files/Directory.h"

class ModObserver;

class ModManager {
   public:
	bool changeContentsDirectory(const fs::path&);

	static ModManager& getInstance() {
		if (_singleton == nullptr) {
			_singleton = new ModManager();
		}

		return *_singleton;
	};

	void newMod();

	bool loadFile(std::string filepath);
	bool saveFile(std::string filepath);

	Directory& getRootDir() const { return *_rootDir; };

	void addObserver(ModObserver* observer) { _observers.insert(observer); };
	void removeObserver(ModObserver* observer) { _observers.erase(observer); }

	void alertObservers();

   private:
	static ModManager* _singleton;

	ModManager() = default;

	Mod* _currentMod = nullptr;

	// fs::path _contentsDirPath;
	Directory* _rootDir;

	std::string _currentContentsDirectory;

	std::set<ModObserver*> _observers;
};
