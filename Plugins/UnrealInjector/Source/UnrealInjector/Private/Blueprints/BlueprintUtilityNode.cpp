// "Unreal Pokémon" created by Retro & Chill.


#include "Blueprints/BlueprintUtilityNode.h"
#include "Blueprints/UtilityNodeSubsystem.h"

void UBlueprintUtilityNode::Destruct() {
    CastChecked<UUtilityNodeSubsystem>(GetOuter())->DestroyNode(this);
}