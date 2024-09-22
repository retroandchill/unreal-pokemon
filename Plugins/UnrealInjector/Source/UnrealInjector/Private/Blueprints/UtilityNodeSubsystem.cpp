// "Unreal Pokémon" created by Retro & Chill.

#include "Blueprints/UtilityNodeSubsystem.h"
#include "DependencyInjectionSettings.h"

void UUtilityNodeSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);
    for (auto &[Key, Value] : GetDefault<UDependencyInjectionSettings>()->BlueprintNodeClasses) {
        NodeTypes.Emplace(Key.TryLoadClass<UBlueprintUtilityNode>(), Value.TryLoadClass<UBlueprintUtilityNode>());
    }
}

void UUtilityNodeSubsystem::DestroyNode(UBlueprintUtilityNode *Object) {
    CreatedNodes.Remove(Object);
}