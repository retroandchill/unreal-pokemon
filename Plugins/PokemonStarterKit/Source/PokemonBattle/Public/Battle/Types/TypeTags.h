// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace Pokemon::Battle::Types {

POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SingleTypeModifierEvent)
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(FullTypeMatchUpEvent)
POKEMONBATTLE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BattlerTypes)

constexpr auto AttackingTagsFormat = TEXT("Battle.Battler.Types.Attacking.{0}");
constexpr auto DefendingTagsFormat = TEXT("Battle.Battler.Types.Defending.{0}");
constexpr auto MoveTypeUserFormat = TEXT("Battle.UsingMove.Type.{0}");

constexpr auto WeakMatchUpFormat = TEXT("Battle.Battler.Types.MatchUp.Weak.{0}");
constexpr auto ResistMatchUpFormat = TEXT("Battle.Battler.Types.MatchUp.Resist.{0}");
constexpr auto ImmuneMatchUpFormat = TEXT("Battle.Battler.Types.MatchUp.Immune.{0}");

class POKEMONBATTLE_API FLookup {
    FLookup();
    ~FLookup();

public:
    static FLookup& GetInstance();

    const FGameplayTag &GetAttackingTag(FName TagName, bool bErrorOnFail = true);
    const FGameplayTag &GetDefendingTag(FName TagName, bool bErrorOnFail = true);
    const FGameplayTag &GetMoveTypeUserTag(FName TagName, bool bErrorOnFail = true);
    
    const FGameplayTag &GetWeaknessTag(FName TagName, bool bErrorOnFail = true);
    const FGameplayTag &GetResistanceTag(FName TagName, bool bErrorOnFail = true);
    const FGameplayTag &GetImmunityTag(FName TagName, bool bErrorOnFail = true);

private:
    TMap<FName, FGameplayTag> AttackingTags;
    TMap<FName, FGameplayTag> DefendingTags;
    TMap<FName, FGameplayTag> MoveTypeUserTags;
    
    TMap<FName, FGameplayTag> WeaknessTags;
    TMap<FName, FGameplayTag> ResistanceTags;
    TMap<FName, FGameplayTag> ImmunityTags;
    
};

}