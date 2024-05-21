// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CommandCondition.generated.h"

/**
 * Condition for a command in the game.
 */
UCLASS(Abstract, Blueprintable)
class RPGMENUS_API UCommandCondition : public UObject {
    GENERATED_BODY()

public:
    /**
    * Get if this command should actually be added to the window
    * @return Will this command be displayed?
    */
    UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = Commands)
    bool IsEnabled() const;

};
