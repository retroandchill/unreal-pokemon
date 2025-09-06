#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUnrealPokemonEditor, Log, All)

class FUnrealPokemonEditorModule : public IModuleInterface
{
  public:
    void StartupModule() override;
    void ShutdownModule() override;

  private:
    /**
     * Called after the engine is fully initialized
     */
    void OnPostEngineInit() const;
};
