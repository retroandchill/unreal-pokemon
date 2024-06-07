// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameSettings.h"

class UPokemonKitSettings;

namespace Pokemon {
/**
 * The singleton used to retrieve all of the settings in the game
 */
class POKEMONSETTINGS_API FBaseSettings : public IGameSettings {
    FBaseSettings();
    ~FBaseSettings() override = default;

public:
    /**
     * Get the singleton instance that contains all the settings used by the game.
     * @return The settings that are retrieved.
     */
    static const FBaseSettings& Get();

    int32 GetMaxMoney() const final;
    uint8 GetMaxPlayerNameSize() const final;
    
    int32 GetMaximumLevel() const final;
    int32 GetEggLevel() const final;
    int32 GetShinyPokemonChance() const final;
    
    int32 GetMaxPartySize() const final;
    
    const TMap<uint8, FName> & GetPocketNames() const final;
    const TMap<FName, FPocketInfo> & GetPocketInfo() const final;
    int32 GetMaxItemsPerSlot() const final;

private:
    TStrongObjectPtr<const UPokemonKitSettings> KitSettings;
};
}