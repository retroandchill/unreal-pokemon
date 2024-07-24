// "Unreal Pokémon" created by Retro & Chill.

#include "DependencyInjectionSettings.h"

#ifdef WITH_METADATA
void UDependencyInjectionSettings::RefreshDependencies() {
    if (CheckForNewInjectableInterfaces()) {
        TryUpdateDefaultConfigFile();
    }
}
#endif

const TMap<UClass *, FInjectionTarget> &UDependencyInjectionSettings::GetTargetInjections() const {
    return TargetInjections;
}

#ifdef WITH_METADATA
bool UDependencyInjectionSettings::CheckForNewInjectableInterfaces() {
    bool bChangeOccurred = false;
    for (TObjectIterator<UClass> It; It; ++It) {
        if (!It->IsChildOf(UInterface::StaticClass()) || !It->HasMetaData(TEXT("Injectable")) ||
            TargetInjections.Contains(*It)) {
            continue;
        }

        TargetInjections.Emplace(*It, *It);
        bChangeOccurred = true;
    }

    return bChangeOccurred;
}
#endif