#pragma once

#include "Mod.h"
#include "ModManager.h"

struct PathChangedData {
    fs::path path;
    Override* override;
};

class ModObserver {
   public:
    ModObserver();
    virtual ~ModObserver();

    friend void ModManager::alertObservers(ALERT_TYPE type);

   private:
    virtual void onModChanged(const Mod& mod) {};
    virtual void onSelectedPathChanged(const PathChangedData& data) {};
    virtual void onOverrideUpdated(const Override* override) {};
};
