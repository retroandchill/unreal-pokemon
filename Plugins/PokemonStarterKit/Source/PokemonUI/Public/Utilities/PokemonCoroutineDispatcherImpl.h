// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Utilities/PokemonCoroutineDispatcher.h"

#include "PokemonCoroutineDispatcherImpl.generated.h"

/**
 *
 */
UCLASS()
class POKEMONUI_API UPokemonCoroutineDispatcherImpl : public UObject, public IPokemonCoroutineDispatcher {
    GENERATED_BODY()

  public:
    UE5Coro::TCoroutine<> DisplayMessage(FText Message, FForceLatentCoroutine Coro = {}) const override;

    TMultiCoroutine<int32, FName> DisplayMessageWithChoices(FText Message,
                                                            const TArray<FText> &Choices, FForceLatentCoroutine Coro = {}) const override;

    UE5Coro::TCoroutine<bool>
    DisplayConfirmPrompt(FText Message,
                         FText ConfirmOption = NSLOCTEXT("PokemonUI", "PromptYes", "Yes"),
                         FText CancelOption = NSLOCTEXT("PokemonUI", "PromptNo", "No"), FForceLatentCoroutine Coro = {}) const override;

    UE5Coro::TCoroutine<TOptional<FSelectedPokemonHandle>>
    SelectPokemonFromParty(FForceLatentCoroutine Coro = {}) const override;

    UE5Coro::TCoroutine<TOptional<FSelectedItemHandle>> SelectItemFromBag(
        const FItemFilter &Filter, FForceLatentCoroutine Coro = {}) const override;

    UE5Coro::TCoroutine<bool> PromptReplaceMove(const TScriptInterface<IPokemon>& Pokemon, FMoveHandle Move, FForceLatentCoroutine Coro = {}) const override;
    
    UE5Coro::TCoroutine<bool> GiveItemToPokemon(const FItemHandle &Item,
                                                const TScriptInterface<IPokemon> Pokemon,
                                                int PokemonIndex, FForceLatentCoroutine Coro = {}) const override;
    
    UE5Coro::TCoroutine<bool> TakeItemFromPokemon(const TScriptInterface<IPokemon> &Pokemon, FForceLatentCoroutine Coro = {}) const override;

    UE5Coro::TCoroutine<bool> LearnMove(const TScriptInterface<IPokemon> &Pokemon, FMoveHandle Move, FForceLatentCoroutine Coro = {}) const override;

    UE5Coro::TCoroutine<> ProcessLevelUp(const TScriptInterface<IPokemon> &Pokemon, const FLevelUpStatChanges& StatChanges, FForceLatentCoroutine Coro = {}) const override;
};
