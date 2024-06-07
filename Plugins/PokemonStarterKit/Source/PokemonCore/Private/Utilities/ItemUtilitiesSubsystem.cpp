// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/ItemUtilitiesSubsystem.h"
#include "Settings/BaseSettings.h"

void UItemUtilitiesSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);
    ItemUtilities = NewObject<UObject>(this, Pokemon::FBaseSettings::Get().GetItemUtilitiesClass());
}

void UItemUtilitiesSubsystem::Deinitialize() {
    Super::Deinitialize();
    ItemUtilities = nullptr;
}

UObject * UItemUtilitiesSubsystem::GetItemUtilities() const {
    return ItemUtilities;
}