// "Unreal Pokémon" created by Retro & Chill.


#include "Settings/BaseSettings.h"
#include "PokemonKitSettings.h"

Pokemon::FBaseSettings::FBaseSettings() : KitSettings(NewObject<UPokemonKitSettings>()) {
}

Pokemon::FBaseSettings & Pokemon::FBaseSettings::Get() {
    static FBaseSettings Instance;
    return Instance;
}

int32 Pokemon::FBaseSettings::GetMaxMoney() const {
    return KitSettings->MaxMoney;
}

uint8 Pokemon::FBaseSettings::GetMaxPlayerNameSize() const {
    return KitSettings->MaxPlayerNameSize;
}

int32 Pokemon::FBaseSettings::GetMaximumLevel() const {
    return KitSettings->MaximumLevel;
}

int32 Pokemon::FBaseSettings::GetEggLevel() const {
    return KitSettings->EggLevel;
}

int32 Pokemon::FBaseSettings::GetShinyPokemonChance() const {
    return KitSettings->ShinyPokemonChance;
}

int32 Pokemon::FBaseSettings::GetMaxPartySize() const {
    return KitSettings->MaxPartySize;
}

const TMap<uint8, FName> & Pokemon::FBaseSettings::GetPocketNames() const {
    return KitSettings->PocketNames;
}

const TMap<FName, FPocketInfo> & Pokemon::FBaseSettings::GetPocketInfo() const {
    return KitSettings->PocketInfo;
}

int32 Pokemon::FBaseSettings::GetMaxItemsPerSlot() const {
    return KitSettings->MaxItemsPerSlot;
}