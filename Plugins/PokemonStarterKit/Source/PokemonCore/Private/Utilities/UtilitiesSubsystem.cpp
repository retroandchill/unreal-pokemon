// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/UtilitiesSubsystem.h"
#include "Settings/BaseSettings.h"

void UUtilitiesSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);
    ItemUtilities = NewObject<UObject>(this, Pokemon::FBaseSettings::Get().GetItemUtilitiesClass());
    PokemonUtilities = NewObject<UObject>(this, Pokemon::FBaseSettings::Get().GetPokemonUtilitiesClass());
}

void UUtilitiesSubsystem::Deinitialize() {
    Super::Deinitialize();
    ItemUtilities = nullptr;
}

UObject * UUtilitiesSubsystem::GetPokemonUtilities() const {
    return PokemonUtilities;
}

UObject *UUtilitiesSubsystem::GetItemUtilities() const {
    return ItemUtilities;
}