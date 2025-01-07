// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UE5Coro.h"


namespace Pokemon::UI {

    template <typename... A>
    using TMultiCoroutine = UE5Coro::TCoroutine<TTuple<int32, FName>>;


    POKEMONUI_API UE5Coro::TCoroutine<> DisplayMessage(const UObject* WorldContext, FText Message);

    POKEMONUI_API TMultiCoroutine<int32, FName> DisplayMessageWithChoices(const UObject* WorldContext, FText Message, const TArray<FText> &Choices);

};
