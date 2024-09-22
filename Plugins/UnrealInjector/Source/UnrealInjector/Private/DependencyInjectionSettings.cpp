// "Unreal Pokémon" created by Retro & Chill.

#include "DependencyInjectionSettings.h"

#if WITH_METADATA
void UDependencyInjectionSettings::RefreshDependencies() {
    if (CheckForNewInjectableInterfaces()) {
        TryUpdateDefaultConfigFile();
    }
}
#endif

const TMap<UClass *, FInjectionTarget> &UDependencyInjectionSettings::GetTargetInjections() const {
    return TargetInjections;
}

const TMap<UClass *, FBlueprintNodeTarget> & UDependencyInjectionSettings::GetBlueprintNodeClasses() const {
    return BlueprintNodeClasses;
}

#if WITH_METADATA
bool UDependencyInjectionSettings::CheckForNewInjectableInterfaces() {
    bool bChangeOccurred = false;
    for (TObjectIterator<UClass> It; It; ++It) {
        if (It->IsChildOf(UInterface::StaticClass()) && It->HasMetaData(TEXT("Injectable")) && !TargetInjections.Contains(*It)) {
            TargetInjections.Emplace(*It, *It);
            bChangeOccurred = true;
        } else if (It->HasMetaData(TEXT("UtilityNode")) && !BlueprintNodeClasses.Contains(*It)) {
            BlueprintNodeClasses.Emplace(*It, *It);
            bChangeOccurred = true;
        }
    }

    return bChangeOccurred;
}
#endif