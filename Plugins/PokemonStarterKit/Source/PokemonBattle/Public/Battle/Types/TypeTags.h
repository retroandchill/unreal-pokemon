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

class POKEMONBATTLE_API FLookup {
    FLookup();
    ~FLookup();

public:
    static FLookup& GetInstance();

    FORCEINLINE const FNativeGameplayTag &GetAttackingTag(FName TagName) const {
        return *AttackingTags.FindChecked(TagName);
    }
    
    FORCEINLINE const FNativeGameplayTag &GetDefendingTag(FName TagName) const {
        return *DefendingTags.FindChecked(TagName);
    }
    
    FORCEINLINE const FNativeGameplayTag &GetMoveTypeUserTag(FName TagName) const {
        return *MoveTypeUserTags.FindChecked(TagName);
    }

private:
    TMap<FName, TSharedRef<FNativeGameplayTag>> AttackingTags;
    TMap<FName,  TSharedRef<FNativeGameplayTag>> DefendingTags;
    TMap<FName,  TSharedRef<FNativeGameplayTag>> MoveTypeUserTags;
    
};

}