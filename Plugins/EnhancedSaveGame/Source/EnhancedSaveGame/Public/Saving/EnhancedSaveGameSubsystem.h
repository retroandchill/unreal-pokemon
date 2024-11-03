// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnhancedSaveGameSubsystem.generated.h"

class UEnhancedSaveGame;
struct FGameplayTagContainer;

/**
 * The Enhanced Save Game Subsystem.
 * Provides enhanced save game functionality, extending base save game capabilities.
 */
UCLASS()
class ENHANCEDSAVEGAME_API UEnhancedSaveGameSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

public:
    /**
     * Retrieves the instance of the UEnhancedSaveGameSubsystem associated with the provided world context.
     *
     * @param WorldContext The context from which to retrieve the instance of the subsystem.
     * @return A reference to the UEnhancedSaveGameSubsystem instance.
     */
    static UEnhancedSaveGameSubsystem& Get(const UObject* WorldContext);

    /**
     * Creates a new instance of UEnhancedSaveGame and populates it using data from subsystems that implement ISaveableSubsystem.
     *
     * @param SaveTags The set of gameplay tags used for saving relevant data.
     * @return A pointer to the newly created UEnhancedSaveGame instance.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Saving,
        meta = (AutoCreateRefTerm = SaveTags))
    UEnhancedSaveGame* CreateSaveGame(const FGameplayTagContainer& SaveTags = FGameplayTagContainer()) const;

    /**
     * Loads the save game data into subsystems implementing the ISaveableSubsystem interface.
     *
     * @param SaveGame The save game instance containing the data to be loaded.
     * @param LoadTags The set of gameplay tags used to filter which data should be loaded.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Saving,
        meta = (AutoCreateRefTerm = LoadTags))
    void LoadSaveGame(const UEnhancedSaveGame* SaveGame, const FGameplayTagContainer& LoadTags = FGameplayTagContainer()) const;

private:

};
