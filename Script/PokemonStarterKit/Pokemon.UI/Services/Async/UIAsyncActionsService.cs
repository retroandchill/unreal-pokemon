using InteractiveUI.Core;
using Pokemon.Core.Entities;
using Pokemon.Core.Services.Async;
using Pokemon.Data.Model.PBS;
using UnrealInject.SourceGenerator.Attributes;
using UnrealSharp;
using UnrealSharp.CoreUObject;
using UnrealSharp.Engine;
using UnrealSharp.GameplayTags;

namespace Pokemon.UI.Services.Async;

/// <summary>
/// Provides asynchronous UI services for common actions in the Pokemon UI.
/// This class implements the <see cref="Pokemon.Core.Services.Async.IAsyncActionsService"/> interface.
/// </summary>
[Service]
public class UIAsyncActionsService : IAsyncActionsService
{
    /// <inheritdoc />
    public Task ShowMessageAsync(
        APlayerController playerController,
        FText message,
        CancellationToken cancellationToken = default
    )
    {
        throw new NotImplementedException();
    }

    /// <inheritdoc />
    public Task<int> ShowChoicePrompt(
        APlayerController playerController,
        FText message,
        IReadOnlyList<FText> choices,
        CancellationToken cancellationToken = default
    )
    {
        throw new NotImplementedException();
    }

    /// <inheritdoc />
    public Task<bool> ShowYesNoPromptAsync(
        APlayerController playerController,
        FText message,
        FText yesText,
        FText noText,
        CancellationToken cancellationToken = default
    )
    {
        throw new NotImplementedException();
    }

    /// <inheritdoc />
    public async Task<UPokemon?> SelectPokemonFromPartyAsync(
        APlayerController playerController,
        CancellationToken cancellationToken = default
    )
    {
        var settings = UObject.GetDefault<UPokemonUISettings>();
        var screenClass = settings.PokemonSelectScreenClass;
        if (screenClass.IsNull)
        {
            throw new InvalidOperationException("PokemonSelectScreenClass is not set");
        }

        var pokemonSelectScreen = await playerController.PushContentToLayerAsync(
            GameplayTags.UI_Layer_GameMenu,
            screenClass,
            cancellationToken: cancellationToken
        );

        return await pokemonSelectScreen.SelectPokemonAsync(cancellationToken);
    }

    /// <inheritdoc />
    public Task<FItemSlotInfo?> SelectItemFromBagAsync(
        APlayerController playerController,
        FGameplayTag? pocketFilter = null,
        Func<FItemHandle, bool>? filter = null,
        CancellationToken cancellationToken = default
    )
    {
        throw new NotImplementedException();
    }

    /// <inheritdoc />
    public Task<bool> PromptReplaceMoveAsync(
        APlayerController playerController,
        UPokemon pokemon,
        FMoveHandle move,
        CancellationToken cancellationToken = default
    )
    {
        throw new NotImplementedException();
    }

    /// <inheritdoc />
    public Task<bool> GivePokemonItemFromBagAsync(
        APlayerController playerController,
        UPokemon pokemon,
        CancellationToken cancellationToken = default
    )
    {
        throw new NotImplementedException();
    }

    /// <inheritdoc />
    public Task<bool> GiveItemToPokemonAsync(
        APlayerController playerController,
        UPokemon pokemon,
        FItemHandle item,
        CancellationToken cancellationToken = default
    )
    {
        throw new NotImplementedException();
    }

    /// <inheritdoc />
    public Task<bool> TakeItemFromPokemonAsync(
        APlayerController playerController,
        UPokemon pokemon,
        CancellationToken cancellationToken = default
    )
    {
        throw new NotImplementedException();
    }

    /// <inheritdoc />
    public Task<bool> LearnMoveAsync(
        APlayerController playerController,
        UPokemon pokemon,
        FMoveHandle move,
        CancellationToken cancellationToken = default
    )
    {
        throw new NotImplementedException();
    }
}
