#pragma once

#include "Mod.h"
#include "ModManager.h"

struct PathChangedData {
    fs::path root_path = "";
    fs::path tail_path = "";
    Override* override;

    [[nodiscard]] fs::path get_full_path() const {
        return root_path / tail_path;
    }
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
