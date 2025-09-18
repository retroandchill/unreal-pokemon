using LanguageExt;
using Pokemon.Core.Entities;
using Pokemon.Core.Services.Async;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Engine;
using UnrealSharp.GameplayTags;

namespace Pokemon.Core.Utilities;

public static class PokemonAsyncExtensions
{
    private static IAsyncActionsService GetActionService(APlayerController playerController)
    {
        return playerController.GetGameInstanceSubsystem<UPokemonSubsystem>().AsyncActionsService;
    }

    public static Task ShowMessageAsync(
        this APlayerController playerController,
        FText message,
        CancellationToken cancellationToken = default
    )
    {
        var service = GetActionService(playerController);
        return service.ShowMessageAsync(playerController, message, cancellationToken);
    }

    public static Task<int> ShowChoicePrompt(
        this APlayerController playerController,
        FText message,
        IReadOnlyList<FText> choices,
        CancellationToken cancellationToken = default
    )
    {
        var service = GetActionService(playerController);
        return service.ShowChoicePrompt(playerController, message, choices, cancellationToken);
    }

    public static Task<bool> ShowYesNoPromptAsync(
        this APlayerController playerController,
        FText message,
        FText yesText,
        FText noText,
        CancellationToken cancellationToken = default
    )
    {
        var service = GetActionService(playerController);
        return service.ShowYesNoPromptAsync(
            playerController,
            message,
            yesText,
            noText,
            cancellationToken
        );
    }

    public static Task<UPokemon?> SelectPokemonFromPartyAsync(
        this APlayerController playerController,
        CancellationToken cancellationToken = default
    )
    {
        var service = GetActionService(playerController);
        return service.SelectPokemonFromPartyAsync(playerController, cancellationToken);
    }

    public static Task<FItemSlotInfo?> SelectItemFromBagAsync(
        this APlayerController playerController,
        FGameplayTag? pocketFilter = null,
        Func<FItemHandle, bool>? filter = null,
        CancellationToken cancellationToken = default
    )
    {
        var service = GetActionService(playerController);
        return service.SelectItemFromBagAsync(
            playerController,
            pocketFilter,
            filter,
            cancellationToken
        );
    }

    public static Task<bool> PromptReplaceMoveAsync(
        this APlayerController playerController,
        UPokemon pokemon,
        FMoveHandle move,
        CancellationToken cancellationToken = default
    )
    {
        var service = GetActionService(playerController);
        return service.PromptReplaceMoveAsync(playerController, pokemon, move, cancellationToken);
    }

    public static Task<bool> GivePokemonItemFromBagAsync(
        this APlayerController playerController,
        UPokemon pokemon,
        CancellationToken cancellationToken = default
    )
    {
        var service = GetActionService(playerController);
        return service.GivePokemonItemFromBagAsync(playerController, pokemon, cancellationToken);
    }

    public static Task<bool> GiveItemToPokemonAsync(
        this APlayerController playerController,
        UPokemon pokemon,
        FItemHandle item,
        CancellationToken cancellationToken = default
    )
    {
        var service = GetActionService(playerController);
        return service.GiveItemToPokemonAsync(playerController, pokemon, item, cancellationToken);
    }

    public static Task<bool> TakeItemFromPokemonAsync(
        this APlayerController playerController,
        UPokemon pokemon,
        CancellationToken cancellationToken = default
    )
    {
        var service = GetActionService(playerController);
        return service.TakeItemFromPokemonAsync(playerController, pokemon, cancellationToken);
    }

    public static Task<bool> LearnMoveAsync(
        this APlayerController playerController,
        UPokemon pokemon,
        FMoveHandle move,
        CancellationToken cancellationToken = default
    )
    {
        var service = GetActionService(playerController);
        return service.LearnMoveAsync(playerController, pokemon, move, cancellationToken);
    }
}
