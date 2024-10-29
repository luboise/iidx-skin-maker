#include "ModObserver.h"

#include "ModManager.h"

ModObserver::ModObserver() { ModManager::getInstance().addObserver(this); }
ModObserver::~ModObserver() { ModManager::getInstance().removeObserver(this); }
