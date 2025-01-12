// "Unreal Pokémon" created by Retro & Chill.

#include "Field/FieldItemEffectUseOnPokemon.h"

UE5Coro::TCoroutine<bool> UFieldItemEffectUseOnPokemon::UseItem(const FItem &Item, int32 Quantity,
                                                                TScriptInterface<IPokemon> Pokemon) {
    auto [Result] =
        co_await Retro::BindToDelegateDispatch(GetOnEffectComplete(), [&] { Use(Item, Quantity, Pokemon); });
    co_return Result;
}