// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnhancedSaveGameModule.h"

DEFINE_LOG_CATEGORY(LogEnhancedSaveGame);

FEnhancedSaveGameModule *FEnhancedSaveGameModule::Instance = nullptr;

void FEnhancedSaveGameModule::StartupModule() {
    check(Instance == nullptr)
    Instance = this;
}

void FEnhancedSaveGameModule::ShutdownModule() {
    check(Instance != nullptr)
    Instance = nullptr;
}

ISaveGameSystem *FEnhancedSaveGameModule::GetSaveGameSystem() {
#if WITH_EDITOR
    return bInMemorySavingEnabled ? static_cast<ISaveGameSystem *>(&InMemorySaveGameSystem)
                                  : static_cast<ISaveGameSystem *>(&MainSaveGameSystem);
#else
    return &MainSaveGameSystem;
#endif
}

FEnhancedSaveGameModule &FEnhancedSaveGameModule::Get() {
    check(Instance != nullptr)
    return *Instance;
}

#if WITH_EDITOR
void FEnhancedSaveGameModule::EnableInMemorySaving() {
    bInMemorySavingEnabled = true;
}

void FEnhancedSaveGameModule::DisableInMemorySaving() {
    bInMemorySavingEnabled = false;
}
#endif

IMPLEMENT_MODULE(FEnhancedSaveGameModule, EnhancedSaveGame)