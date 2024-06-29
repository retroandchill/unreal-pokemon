// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Types/TypeTags.h"
#include "DataManager.h"
#include "Battle/Type.h"

namespace Pokemon::Battle::Types {

UE_DEFINE_GAMEPLAY_TAG(SingleTypeModifierEvent, "Battle.Damage.TypeMatchup.Single")
UE_DEFINE_GAMEPLAY_TAG(FullTypeMatchUpEvent, "Battle.Damage.TypeMatchup.Full")
UE_DEFINE_GAMEPLAY_TAG(BattlerTypes, "Battle.Battler.Types")

static void AddDynamicGameplayTag(TMap<FName, TSharedRef<FNativeGameplayTag>>& Tags, FStringView Format, FName ID) {
    auto TagName = FString::Format(Format.GetData(), { ID.ToString() });
    auto Tag = MakeShared<FNativeGameplayTag>(UE_PLUGIN_NAME, UE_MODULE_NAME, *TagName, TEXT(""), ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD);
    Tags.Emplace(ID, MoveTemp(Tag));
}

FLookup::FLookup() {
    auto &DataManager = FDataManager::GetInstance();
    auto& TypeTable = DataManager.GetDataTable<FType>();
    TypeTable.ForEach([this](const FType& Type) {
        // Add the tags for the types you attack from and defend from
        if (!Type.IsPseudoType) {
            AddDynamicGameplayTag(AttackingTags, AttackingTagsFormat, Type.ID);
            AddDynamicGameplayTag(DefendingTags, DefendingTagsFormat, Type.ID);
        }

        AddDynamicGameplayTag(MoveTypeUserTags, MoveTypeUserFormat, Type.ID);
    });
}

FLookup::~FLookup() = default;

FLookup & FLookup::GetInstance() {
    static FLookup Lookup;
    return Lookup;
}

}