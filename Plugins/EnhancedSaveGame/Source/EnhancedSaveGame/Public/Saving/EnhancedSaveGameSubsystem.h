// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnhancedSaveGameSubsystem.generated.h"

class UEnhancedSaveGame;
/**
 * 
 */
UCLASS()
class ENHANCEDSAVEGAME_API UEnhancedSaveGameSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Saving)
    UEnhancedSaveGame* CreateSaveGame() const;

private:

};
