// "Unreal Pokémon" created by Retro & Chill.


#include "Blueprints/UtilityNodeSubsystem.h"
#include "DependencyInjectionSettings.h"

void UUtilityNodeSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);
    for (auto &[Key, Value] : GetDefault<UDependencyInjectionSettings>()->GetBlueprintNodeClasses()) {
        NodeTypes.Emplace(Key, Value.InjectedClass.TryLoadClass<UObject>());
    }
}

void UUtilityNodeSubsystem::DestroyNode(UObject *Object) {
    CreatedNodes.Remove(Object);
}