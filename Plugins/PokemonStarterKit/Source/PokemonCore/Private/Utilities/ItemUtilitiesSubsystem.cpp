// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/ItemUtilitiesSubsystem.h"
#include "Settings/BagSettings.h"

void UItemUtilitiesSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);
    ItemUtilities = NewObject<UObject>(this, GetDefault<UBagSettings>()->GetItemUtilitiesClass().TryLoadClass<UObject>());
}

void UItemUtilitiesSubsystem::Deinitialize() {
    Super::Deinitialize();
    ItemUtilities = nullptr;
}

UObject * UItemUtilitiesSubsystem::GetItemUtilities() const {
    return ItemUtilities;
}