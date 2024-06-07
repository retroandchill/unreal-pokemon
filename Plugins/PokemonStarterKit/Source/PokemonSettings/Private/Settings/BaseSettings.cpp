// "Unreal Pokémon" created by Retro & Chill.


#include "Settings/BaseSettings.h"
#include "PokemonKitSettings.h"
#include "Algo/ForEach.h"
#include "Mainpulation/RangeHelpers.h"

namespace Pokemon {
struct FBaseSettingsPrivate {

    TStrongObjectPtr<const UPokemonKitSettings> KitSettings = TStrongObjectPtr(NewObject<UPokemonKitSettings>());
};
}


Pokemon::FBaseSettings::FBaseSettings() : InternalData(MakeUnique<FBaseSettingsPrivate>()) {
}

Pokemon::FBaseSettings::~FBaseSettings() = default;

Pokemon::FBaseSettings & Pokemon::FBaseSettings::Get() {
    static FBaseSettings Instance;
    return Instance;
}

int32 Pokemon::FBaseSettings::GetMaxMoney() const {
    return InternalData->KitSettings->MaxMoney;
}

uint8 Pokemon::FBaseSettings::GetMaxPlayerNameSize() const {
    return InternalData->KitSettings->MaxPlayerNameSize;
}

int32 Pokemon::FBaseSettings::GetMaximumLevel() const {
    return InternalData->KitSettings->MaximumLevel;
}

int32 Pokemon::FBaseSettings::GetEggLevel() const {
    return InternalData->KitSettings->EggLevel;
}

int32 Pokemon::FBaseSettings::GetMaxDefaultAbilities() const {
    return InternalData->KitSettings->MaxDefaultAbilities;
}

int32 Pokemon::FBaseSettings::GetShinyPokemonChance() const {
    return InternalData->KitSettings->ShinyPokemonChance;
}

int32 Pokemon::FBaseSettings::GetMaxPartySize() const {
    return InternalData->KitSettings->MaxPartySize;
}

const TMap<uint8, FName> & Pokemon::FBaseSettings::GetPocketNames() const {
    return InternalData->KitSettings->PocketNames;
}

const TMap<FName, FPocketInfo> & Pokemon::FBaseSettings::GetPocketInfo() const {
    return InternalData->KitSettings->PocketInfo;
}

int32 Pokemon::FBaseSettings::GetMaxItemsPerSlot() const {
    return InternalData->KitSettings->MaxItemsPerSlot;
}

void Pokemon::FBaseSettings::ForEachDataTable(const TFunctionRef<void(UDataTable*)>& Callback) const {
    auto Range = RangeHelpers::CreateRange(InternalData->KitSettings->DataTables)
        | std::views::transform([](const FSoftObjectPath& Path) { return Path.TryLoad(); })
        | std::views::transform([](UObject* Object) { return CastChecked<UDataTable>(Object); });
    std::ranges::for_each(Range, Callback);
}

void Pokemon::FBaseSettings::LoadDataTables() const {
    return Algo::ForEach(InternalData->KitSettings->DataTables, [](const FSoftObjectPath& Path) { return Path.TryLoad(); });
}