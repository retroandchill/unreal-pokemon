// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UE5Coro.h"

namespace Pokemon::Testing {
    POKEMONTESTS_API UE5Coro::TCoroutine<> AdvanceMessages(const UObject *WorldContextObject,
                                                           FForceLatentCoroutine Coro = {});
    POKEMONTESTS_API UE5Coro::TCoroutine<> AdvanceMessagesUntilPrompt(const UObject *WorldContextObject,
                                                                      int32 IndexToSelect = 0,
                                                                      FForceLatentCoroutine Coro = {});
} // namespace Pokemon::Testing
