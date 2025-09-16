// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Items/ItemLookup.h"
#include "Abilities/GameplayAbility.h"
#include "Bag/Item.h"
#include "Battle/BlueprintClasses.h"
#include "Battle/Settings/PokemonBattleSettings.h"
#include "RetroLib/Optionals/AndThen.h"
#include "RetroLib/Optionals/OrElseGet.h"
#include "RetroLib/Optionals/OrElseValue.h"
#include "RetroLib/Optionals/Transform.h"

TOptional<TNonNullSubclassOf<UGameplayAbility>> Pokemon::Battle::Items::FindHoldItemEffect(FName ID)
{
    return {}; //Classes::HoldItemEffects.LoadClass(ID);
}

TOptional<TNonNullSubclassOf<UGameplayAbility>> Pokemon::Battle::Items::FindHoldItemEffect(const FItem *Item)
{
    // clang-format off
    return Retro::Optionals::OfNullable(Item) |
           Retro::Optionals::Transform(&FItem::ID) |
           Retro::Optionals::AndThen([](FName ID) {
               return FindHoldItemEffect(ID);
           });
    // clang-format on
}

TSubclassOf<UBattleItemEffect> Pokemon::Battle::Items::FindBattleItemEffect(FName ID)
{
    // clang-format off
    return nullptr;
    /*
    return Classes::ItemEffects.LoadClass(ID) |
           Retro::Optionals::OrElseValue(UBattleItemEffect::StaticClass());
           */
    // clang-format on
}

TSubclassOf<UBattleItemEffect> Pokemon::Battle::Items::FindBattleItemEffect(const FItem *Item)
{
    // clang-format off
    auto ID = Retro::Optionals::OfNullable(Item) |
              Retro::Optionals::Transform(&FItem::ID) |
              Retro::Optionals::OrElseValue(NAME_None);
    // clang-format on

    return FindBattleItemEffect(ID);
}