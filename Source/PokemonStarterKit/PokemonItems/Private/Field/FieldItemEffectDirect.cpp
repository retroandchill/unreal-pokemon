// "Unreal Pokémon" created by Retro & Chill.

#include "Field/FieldItemEffectDirect.h"

UE5Coro::TCoroutine<bool> UFieldItemEffectDirect::UseItem(const FItem &Item, int32 Quantity)
{
    auto [Result] = co_await Retro::BindToDelegateDispatch(GetOnEffectComplete(), [&] { Use(Item, Quantity); });
    co_return Result;
}