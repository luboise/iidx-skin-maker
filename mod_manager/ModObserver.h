#pragma once

#include "Mod.h"
#include "ModManager.h"

class ModObserver {
   public:
	ModObserver();

	friend void ModManager::alertObservers();

   private:
	virtual void onModChanged(const Mod&) = 0;
};
