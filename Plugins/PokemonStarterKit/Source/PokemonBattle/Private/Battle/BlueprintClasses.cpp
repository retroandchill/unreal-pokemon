// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/BlueprintClasses.h"

namespace Pokemon::Battle::Classes {
    UE_DEFINE_BLUEPRINT_CLASS(MoveEffects, UBattleMoveFunctionCode,
        "/Game/Blueprints/Battle/Moves/FunctionCodes", "Move_");
    
    UE_DEFINE_BLUEPRINT_CLASS(ItemEffects, UBattleItemEffect,
        "/Game/Blueprints/Battle/Items/Usable/Effects", "BattleItem_");
    
    UE_DEFINE_BLUEPRINT_CLASS(AbilityEffects, UGameplayAbility,
        "/Game/Blueprints/Battle/Abilities/Effects", "Ability_");

    UE_DEFINE_BLUEPRINT_CLASS(HoldItemEffects, UGameplayAbility,
        "/Game/Blueprints/Battle/Items/HoldItems/Effects", "HoldItem_");

    UE_DEFINE_BLUEPRINT_CLASS(StatusEffects, UGameplayEffect,
        "/Game/Blueprints/Battle/StatusEffects", "StatusEffect_");
}