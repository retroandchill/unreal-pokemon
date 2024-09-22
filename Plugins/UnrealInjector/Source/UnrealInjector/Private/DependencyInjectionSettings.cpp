// "Unreal Pokémon" created by Retro & Chill.

#include "DependencyInjectionSettings.h"

#if WITH_METADATA
void UDependencyInjectionSettings::RefreshDependencies() {
    if (CheckForNewInjectableInterfaces()) {
        TryUpdateDefaultConfigFile();
    }
}
#endif

#if WITH_METADATA
bool UDependencyInjectionSettings::CheckForNewInjectableInterfaces() {
    auto ContainsType = [](const TArray<FInjectionTarget>& Collection, UClass* SourceClass) {
      return Collection.ContainsByPredicate([&SourceClass](const FInjectionTarget& Target) {
          return Target.TargetInterface == SourceClass;
      });
    };
    
    bool bChangeOccurred = false;
    for (TObjectIterator<UClass> It; It; ++It) {
        if (It->IsChildOf(UInterface::StaticClass()) && It->HasMetaData(TEXT("Injectable")) && !ContainsType(TargetInjections, *It)) {
            TargetInjections.Emplace(*It);
            bChangeOccurred = true;
        } else if (It->HasMetaData(TEXT("UtilityNode")) && !ContainsType(BlueprintNodeClasses, *It)) {
            BlueprintNodeClasses.Emplace(*It);
            bChangeOccurred = true;
        }
    }

    return bChangeOccurred;
}
#endif