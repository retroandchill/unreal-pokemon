// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "SaveGameSystem.h"
#include "Saving/SaveGameManager/InMemorySaveGameSystem.h"

DECLARE_LOG_CATEGORY_EXTERN(LogEnhancedSaveGame, Log, All)

class ENHANCEDSAVEGAME_API FEnhancedSaveGameModule : public ISaveGameSystemModule
{
  public:
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
    ISaveGameSystem *GetSaveGameSystem() override;

    static FEnhancedSaveGameModule &Get();

#if WITH_EDITOR
    void EnableInMemorySaving();
    void DisableInMemorySaving();
#endif

  private:
    FGenericSaveGameSystem MainSaveGameSystem;

#if WITH_EDITOR
    EnhancedSaveGame::FInMemorySaveGameSystem InMemorySaveGameSystem;
    bool bInMemorySavingEnabled = false;
#endif

    static FEnhancedSaveGameModule *Instance;
};