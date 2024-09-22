// "Unreal Pokémon" created by Retro & Chill.


#include "Blueprints/BlueprintUtilityNode.h"
#include "Blueprints/UtilityNodeSubsystem.h"

bool UBlueprintUtilityNode::ImplementsGetWorld() const {
    return true;
}

void UBlueprintUtilityNode::Destruct() {
    CastChecked<UUtilityNodeSubsystem>(GetOuter())->DestroyNode(this);
}