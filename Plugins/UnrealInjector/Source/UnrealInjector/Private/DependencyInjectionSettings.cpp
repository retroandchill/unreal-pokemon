// "Unreal Pokémon" created by Retro & Chill.

#include "DependencyInjectionSettings.h"
#include "Lookup/InjectionUtilities.h"

UDependencyInjectionSettings::UDependencyInjectionSettings(const FObjectInitializer &ObjectInitializer)
    : UDeveloperSettings(ObjectInitializer) {
#ifdef WITH_METADATA
    for (TObjectIterator<UClass> It; It; ++It) {
        if (!It->IsChildOf(UInterface::StaticClass()) || !It->HasMetaData(TEXT("Injectable")) ||
            TargetInjections.Contains(*It)) {
            continue;
        }

        TargetInjections.Emplace(*It, *It);
    }
#endif
}

const TMap<UClass *, FInjectionTarget> &UDependencyInjectionSettings::GetTargetInjections() const {
    return TargetInjections;
}