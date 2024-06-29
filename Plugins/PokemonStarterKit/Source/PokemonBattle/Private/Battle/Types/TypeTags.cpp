// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Types/TypeTags.h"

namespace Pokemon::Battle::Types {

UE_DEFINE_GAMEPLAY_TAG(SingleTypeModifierEvent, "Battle.Damage.TypeMatchup.Single")
UE_DEFINE_GAMEPLAY_TAG(FullTypeMatchUpEvent, "Battle.Damage.TypeMatchup.Full")
UE_DEFINE_GAMEPLAY_TAG(BattlerTypes, "Battle.Battler.Types")

static const FGameplayTag &GetOrLookupTag(TMap<FName, FGameplayTag>& Map, FName TagName, FStringView NameFormat, bool bErrorOnFail) {
    if (auto Existing = Map.Find(TagName); Existing != nullptr) {
        return *Existing;
    }

    auto TagString = FString::Format(NameFormat.GetData(), { TagName.ToString() });
    return Map.Add(TagName, FGameplayTag::RequestGameplayTag(*TagString, bErrorOnFail));
}

FLookup::FLookup() = default;

FLookup::~FLookup() = default;

FLookup & FLookup::GetInstance() {
    static FLookup Lookup;
    return Lookup;
}

const FGameplayTag &FLookup::GetAttackingTag(FName TagName, bool bErrorOnFail) {
    return GetOrLookupTag(AttackingTags, TagName, AttackingTagsFormat, bErrorOnFail);
}

const FGameplayTag &FLookup::GetDefendingTag(FName TagName, bool bErrorOnFail) {
    return GetOrLookupTag(DefendingTags, TagName, DefendingTagsFormat, bErrorOnFail);
}

const FGameplayTag &FLookup::GetMoveTypeUserTag(FName TagName, bool bErrorOnFail) {
    return GetOrLookupTag(MoveTypeUserTags, TagName, MoveTypeUserFormat, bErrorOnFail);
}

const FGameplayTag & FLookup::GetWeaknessTag(FName TagName, bool bErrorOnFail) {
    return GetOrLookupTag(WeaknessTags, TagName, WeakMatchUpFormat, bErrorOnFail);
}

const FGameplayTag & FLookup::GetResistanceTag(FName TagName, bool bErrorOnFail) {
    return GetOrLookupTag(ResistanceTags, TagName, ResistMatchUpFormat, bErrorOnFail);
}

const FGameplayTag & FLookup::GetImmunityTag(FName TagName, bool bErrorOnFail) {
    return GetOrLookupTag(ImmunityTags, TagName, ImmuneMatchUpFormat, bErrorOnFail);
}
}