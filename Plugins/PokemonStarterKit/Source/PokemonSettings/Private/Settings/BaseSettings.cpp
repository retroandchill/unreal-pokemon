// "Unreal Pokémon" created by Retro & Chill.

#include "Settings/BaseSettings.h"
#include "Algo/ForEach.h"
#include "PokemonKitSettings.h"
#include "RangeHelpers.h"
#include <range/v3/view/transform.hpp>

Pokemon::FBaseSettings::FBaseSettings() : KitSettings(GetDefault<UPokemonKitSettings>()) {
}

Pokemon::FBaseSettings::~FBaseSettings() = default;

Pokemon::FBaseSettings &Pokemon::FBaseSettings::Get() {
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

FName Pokemon::FBaseSettings::GetHPStat() const {
    return KitSettings->HPStat;
}

int32 Pokemon::FBaseSettings::GetMaxMoves() const {
    return KitSettings->MaxMoves;
}

int32 Pokemon::FBaseSettings::GetMaxDefaultAbilities() const {
    return KitSettings->MaxDefaultAbilities;
}

int32 Pokemon::FBaseSettings::GetShinyPokemonChance() const {
    return KitSettings->ShinyPokemonChance;
}

FName Pokemon::FBaseSettings::GetDefaultPokeBall() const {
    return KitSettings->DefaultPokeBall;
}

int32 Pokemon::FBaseSettings::GetMaxPartySize() const {
    return KitSettings->MaxPartySize;
}

const TMap<uint8, FName> &Pokemon::FBaseSettings::GetPocketNames() const {
    return KitSettings->PocketNames;
}

const TMap<FName, FPocketInfo> &Pokemon::FBaseSettings::GetPocketInfo() const {
    return KitSettings->PocketInfo;
}

int32 Pokemon::FBaseSettings::GetMaxItemsPerSlot() const {
    return KitSettings->MaxItemsPerSlot;
}

const FText &Pokemon::FBaseSettings::GetNoAbilityName() const {
    return KitSettings->NoAbilityName;
}

const FText &Pokemon::FBaseSettings::GetNoAbilityDescription() const {
    return KitSettings->NoAbilityDescription;
}

const TArray<FStatStageInfo> & Pokemon::FBaseSettings::GetStatStages() const {
    return KitSettings->StatStages;
}

const TArray<int32> & Pokemon::FBaseSettings::GetCriticalHitRatios() const {
    return KitSettings->CriticalHitRatios;
}

UClass *Pokemon::FBaseSettings::GetItemUtilitiesClass() const {
    return KitSettings->ItemUtilitiesClass.TryLoadClass<UObject>();
}

void Pokemon::FBaseSettings::ForEachDataTable(const TFunctionRef<void(UDataTable *)> &Callback) const {
    auto Range = RangeHelpers::CreateRange(KitSettings->DataTables) |
                 ranges::views::transform([](const FSoftObjectPath &Path) { return Path.TryLoad(); }) |
                 ranges::views::transform([](UObject *Object) { return CastChecked<UDataTable>(Object); });
    std::ranges::for_each(Range, Callback);
}

const FPokemonSpriteSettings &Pokemon::FBaseSettings::GetPokemonSpriteSettings() const {
    return KitSettings->PokemonSprites;
}

const FTrainerSpriteSettings &Pokemon::FBaseSettings::GetTrainerSpriteSettings() const {
    return KitSettings->TrainerSprites;
}

const FSpriteRepositories &Pokemon::FBaseSettings::GetSpriteRepositories() const {
    return KitSettings->SpriteRepositories;
}

const FDynamicAssetPaths &Pokemon::FBaseSettings::GetDynamicAssetPaths() const {
    return KitSettings->DynamicAssetPaths;
}
