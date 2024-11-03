// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnhancedSaveGameSubsystem.generated.h"

class UEnhancedSaveGame;
struct FGameplayTagContainer;
/**
 * 
 */
UCLASS()
class ENHANCEDSAVEGAME_API UEnhancedSaveGameSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Saving,
        meta = (AutoCreateRefTerm = SaveTags))
    UEnhancedSaveGame* CreateSaveGame(const FGameplayTagContainer& SaveTags = FGameplayTagContainer()) const;

private:

};
