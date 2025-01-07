// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "UE5Coro.h"

class USaveGame;

namespace Retro {
    RETROLIB_API UE5Coro::TCoroutine<bool> SaveGameToSlotAsync(USaveGame* SaveGameObject, const FString& SlotName, const int32 UserIndex);
}