// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/UtilitiesSubsystem.h"
#include "Settings/PokemonUtilityClassSettings.h"

void UUtilitiesSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);
    auto &Settings = *GetDefault<UPokemonUtilityClassSettings>();
    ItemUtilities = NewObject<UObject>(this, Settings.ItemUtilitiesClass.TryLoadClass<UObject>());
    PokemonUtilities = NewObject<UObject>(this, Settings.PokemonUtilitiesClass.TryLoadClass<UObject>());
}

void UUtilitiesSubsystem::Deinitialize() {
    Super::Deinitialize();
    ItemUtilities = nullptr;
}

UObject *UUtilitiesSubsystem::GetPokemonUtilities() const {
    return PokemonUtilities;
}

UObject *UUtilitiesSubsystem::GetItemUtilities() const {
    return ItemUtilities;
}