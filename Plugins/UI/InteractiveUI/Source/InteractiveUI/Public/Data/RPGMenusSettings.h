// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "RPGMenusSettings.generated.h"

/**
 * The settings class for the RPG Menus module.
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "RPG Menus"))
class INTERACTIVEUI_API URPGMenusSettings : public UDeveloperSettings
{
    GENERATED_BODY()

  public:
    UPROPERTY(EditDefaultsOnly, Config, Category = Input, meta = (AllowedClasses = "InputMappingContext"))
    FSoftObjectPath MenuMappingContext;

    UPROPERTY(EditDefaultsOnly, Config, Category = Input)
    int32 MenuMappingPriority = 100;
};
