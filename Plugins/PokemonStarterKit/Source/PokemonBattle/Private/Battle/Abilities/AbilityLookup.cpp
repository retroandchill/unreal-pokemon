// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Abilities/AbilityLookup.h"
#include "Abilities/GameplayAbility.h"
#include "DynamicAssetLoadingSettings.h"
#include "Battle/BlueprintClasses.h"
#include "Battle/Moves/MoveTags.h"

TOptional<TNonNullSubclassOf<UGameplayAbility>> Pokemon::Battle::Abilities::CreateAbilityEffect(FName ID) {
    return Classes::AbilityEffects.LoadClass(ID);
}