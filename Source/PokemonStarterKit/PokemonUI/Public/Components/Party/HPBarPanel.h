// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "HPBarPanel.generated.h"

class UProgressBar;
// This class does not need to be modified.
UINTERFACE(DisplayName = "HP Bar Panel")
class UHPBarPanel : public UInterface
{
    GENERATED_BODY()
};

/**
 * Panel that displays an HP bar to the player.
 */
class POKEMONUI_API IHPBarPanel
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    UFUNCTION(BlueprintNativeEvent, DisplayName = "Get HP Bar", Category = Widgets)
    UProgressBar *GetHPBar();

    UFUNCTION(BlueprintNativeEvent, DisplayName = "Update HP Bar Percent", Category = Animations)
    void UpdateHPBarPercent(float Percent, int32 HPValue);
};