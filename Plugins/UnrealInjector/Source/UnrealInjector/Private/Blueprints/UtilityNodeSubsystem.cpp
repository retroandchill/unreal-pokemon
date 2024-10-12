// "Unreal Pokémon" created by Retro & Chill.

#include "Blueprints/UtilityNodeSubsystem.h"
#include "DependencyInjectionSettings.h"

void UUtilityNodeSubsystem::DestroyNode(UBlueprintUtilityNode *Object) {
    CreatedNodes.Remove(Object);
}