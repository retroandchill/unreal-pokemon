using LanguageExt;
using Pokemon.Core.Entities;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Engine;
using UnrealSharp.GameplayTags;

namespace Pokemon.Core.Services.Async;

public readonly record struct FItemSlotInfo(FItemHandle Item, int Quantity);

public interface IAsyncActionsService
{
    Task ShowMessageAsync(
        APlayerController playerController,
        FText message,
        CancellationToken cancellationToken = default
    );

    Task<int> ShowChoicePrompt(
        APlayerController playerController,
        FText message,
        IReadOnlyList<FText> choices,
        CancellationToken cancellationToken = default
    );

    Task<bool> ShowYesNoPromptAsync(
        APlayerController playerController,
        FText message,
        FText yesText,
        FText noText,
        CancellationToken cancellationToken = default
    );

    Task<UPokemon?> SelectPokemonFromPartyAsync(
        APlayerController playerController,
        CancellationToken cancellationToken = default
    );

    Task<FItemSlotInfo?> SelectItemFromBagAsync(
        APlayerController playerController,
        FGameplayTag? pocketFilter = null,
        Func<FItemHandle, bool>? filter = null,
        CancellationToken cancellationToken = default
    );

    Task<bool> PromptReplaceMoveAsync(
        APlayerController playerController,
        UPokemon pokemon,
        FMoveHandle move,
        CancellationToken cancellationToken = default
    );

    Task<bool> GivePokemonItemFromBagAsync(
        APlayerController playerController,
        UPokemon pokemon,
        CancellationToken cancellationToken = default
    );

    Task<bool> GiveItemToPokemonAsync(
        APlayerController playerController,
        UPokemon pokemon,
        FItemHandle item,
        CancellationToken cancellationToken = default
    );

    Task<bool> TakeItemFromPokemonAsync(
        APlayerController playerController,
        UPokemon pokemon,
        CancellationToken cancellationToken = default
    );

    Task<bool> LearnMoveAsync(
        APlayerController playerController,
        UPokemon pokemon,
        FMoveHandle move,
        CancellationToken cancellationToken = default
    );
}
