// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Screens/InventoryScreen.h"
#include "Screens/PartyScreen.h"
#include "UE5Coro.h"
#include "Moves/MoveData.h"

namespace Pokemon::UI {

    template <typename... A>
    using TMultiCoroutine = UE5Coro::TCoroutine<TTuple<int32, FName>>;

    POKEMONUI_API UE5Coro::TCoroutine<> DisplayMessage(const UObject *WorldContext, FText Message, FForceLatentCoroutine Coro = {});

    POKEMONUI_API TMultiCoroutine<int32, FName> DisplayMessageWithChoices(const UObject *WorldContext, FText Message,
                                                                          const TArray<FText> &Choices, FForceLatentCoroutine Coro = {});

    POKEMONUI_API UE5Coro::TCoroutine<bool>
    DisplayConfirmPrompt(const UObject *WorldContext, FText Message,
                         FText ConfirmOption = NSLOCTEXT("PokemonUI", "PromptYes", "Yes"),
                         FText CancelOption = NSLOCTEXT("PokemonUI", "PromptNo", "No"), FForceLatentCoroutine Coro = {});

    POKEMONUI_API UE5Coro::TCoroutine<TOptional<FSelectedPokemonHandle>>
    SelectPokemonFromParty(const UObject *WorldContext, FForceLatentCoroutine Coro = {});

    POKEMONUI_API UE5Coro::TCoroutine<TOptional<FSelectedItemHandle>> SelectItemFromBag(const UObject *WorldContext,
        const FItemFilter &Filter, FForceLatentCoroutine Coro = {});

    POKEMONUI_API UE5Coro::TCoroutine<bool> PromptReplaceMove(const TScriptInterface<IPokemon>& Pokemon, FMoveHandle Move, FForceLatentCoroutine Coro = {});

    
}; // namespace Pokemon::UI
