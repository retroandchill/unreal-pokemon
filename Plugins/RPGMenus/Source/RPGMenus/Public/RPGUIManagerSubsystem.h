// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameUIManagerSubsystem.h"

#include "RPGUIManagerSubsystem.generated.h"

class UScreen;
/**
 * Subsystem for managing the UI of the RPG Menus Plugin.
 */
UCLASS(DisplayName = "RPG UI Manager Subsystem")
class RPGMENUS_API URPGUIManagerSubsystem : public UGameUIManagerSubsystem {
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintPure, Category = Screens)
    UScreen *GetTopScreenOfStack() const;

    UFUNCTION(BlueprintPure, Category = Screens)
    UScreen *GetTopScreenOfOverlay() const;
};
