// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Items/ItemLookup.h"
#include "Abilities/GameplayAbility.h"
#include "Bag/Item.h"
#include "DynamicAssetLoadingSettings.h"
#include "PokemonBattleSettings.h"
#include "Battle/BlueprintClasses.h"
#include "Ranges/Optional/OrElse.h"
#include "Ranges/Optional/OrElseGet.h"

TOptional<TNonNullSubclassOf<UGameplayAbility>> Pokemon::Battle::Items::FindHoldItemEffect(FName ID) {
    return Classes::HoldItemEffects.LoadClass(ID);
}

TOptional<TNonNullSubclassOf<UGameplayAbility>> Pokemon::Battle::Items::FindHoldItemEffect(const FItem *Item) {
    // clang-format off
    return UE::Optionals::OfNullable(Item) |
           UE::Optionals::Map(&FItem::ID) |
           UE::Optionals::FlatMap([](FName ID) {
               return FindHoldItemEffect(ID);
           });
    // clang-format on
}

static TSubclassOf<UBattleItemEffect> FindDefaultItemEffect() {
    auto ItemClass =
        GetDefault<UPokemonBattleSettings>()->DefaultBattleItemAbility.TryLoadClass<UGameplayAbility>();
    check(ItemClass != nullptr)
    return ItemClass;
}

TSubclassOf<UBattleItemEffect> Pokemon::Battle::Items::FindBattleItemEffect(FName ID) {
    // clang-format off
    return Classes::ItemEffects.LoadClass(ID) |
           UE::Optionals::OrElseGet(&FindDefaultItemEffect);
    // clang-format on
}

TSubclassOf<UBattleItemEffect> Pokemon::Battle::Items::FindBattleItemEffect(const FItem *Item) {
    // clang-format off
    auto ID = UE::Optionals::OfNullable(Item) |
              UE::Optionals::Map(&FItem::ID) |
              UE::Optionals::OrElse(NAME_None);
    // clang-format on

    return FindBattleItemEffect(ID);
}