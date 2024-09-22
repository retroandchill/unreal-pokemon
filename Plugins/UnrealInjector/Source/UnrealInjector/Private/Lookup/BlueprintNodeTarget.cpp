// "Unreal Pokémon" created by Retro & Chill.


#include "Lookup/BlueprintNodeTarget.h"

FBlueprintNodeTarget::FBlueprintNodeTarget() = default;

FBlueprintNodeTarget::FBlueprintNodeTarget(const TSubclassOf<UObject> &NodeType) : TargetClass(NodeType) {
}