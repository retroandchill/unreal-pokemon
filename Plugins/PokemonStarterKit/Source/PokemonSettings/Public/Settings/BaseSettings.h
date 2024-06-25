// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameSettings.h"

class UPokemonKitSettings;

namespace Pokemon {
/**
 * The singleton used to retrieve the kit's settings.
 */
class POKEMONSETTINGS_API FBaseSettings : public IGameSettings {
    FBaseSettings();
    ~FBaseSettings() override;

  public:
    /**
     * Get the singleton instance that contains all the settings used by the game.
     * @return The settings that are retrieved.
     */
    static FBaseSettings &Get();

    int32 GetMaxMoney() const final;
    uint8 GetMaxPlayerNameSize() const final;

    int32 GetMaximumLevel() const final;
    int32 GetEggLevel() const final;
    FName GetHPStat() const final;
    int32 GetMaxMoves() const final;
    int32 GetMaxDefaultAbilities() const final;
    int32 GetShinyPokemonChance() const final;
    FName GetDefaultPokeBall() const final;

    int32 GetMaxPartySize() const final;

    const TMap<uint8, FName> &GetPocketNames() const final;
    const TMap<FName, FPocketInfo> &GetPocketInfo() const final;
    int32 GetMaxItemsPerSlot() const final;

    const FText &GetNoAbilityName() const final;
    const FText &GetNoAbilityDescription() const final;

    const TArray<FStatStageInfo> &GetStatStages() const final;
    const TArray<int32> &GetCriticalHitRatios() const final;
    float GetCriticalHitDamageMod() const final;

    /**
     * Get the class that represents the item utilities
     * @return The class to instantiate the object from
     */
    UClass *GetItemUtilitiesClass() const;

    /**
     * Iterate over each data table in settings, load them, and process each one
     * @param Callback The callback to be handled by the loop
     */
    void ForEachDataTable(const TFunctionRef<void(UDataTable *)> &Callback) const;

    /**
     * Preemptively load all the data tables
     */
    void LoadDataTables() const;

    /**
     * Settings for all Pokémon sprites.
     * @return Settings for all Pokémon sprites.
     */
    const FPokemonSpriteSettings &GetPokemonSpriteSettings() const;

    /**
     * Settings for all Trainer sprites.
     * @return Settings for all Trainer sprites.
     */
    const FTrainerSpriteSettings &GetTrainerSpriteSettings() const;

    /**
     * The repositories for all the different sprites
     * @return The repositories for all the different sprites
     */
    const FSpriteRepositories &GetSpriteRepositories() const;

    /**
     * The paths used to dynamically load assets in the game
     * @return The paths used to dynamically load assets in the game
     */
    const FDynamicAssetPaths &GetDynamicAssetPaths() const;

  private:
    TObjectPtr<const UPokemonKitSettings> KitSettings;
};
} // namespace Pokemon