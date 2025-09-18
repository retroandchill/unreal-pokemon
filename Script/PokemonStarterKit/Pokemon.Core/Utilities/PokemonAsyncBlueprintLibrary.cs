using LanguageExt;
using Pokemon.Core.Entities;
using Pokemon.Core.Services.Async;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.Engine;
using UnrealSharp.GameplayTags;

namespace Pokemon.Core.Utilities;

[UEnum]
public enum ESelectionResult : byte
{
    Selected,
    Cancelled,
}

[UStruct]
public readonly partial record struct FSelectedPokemon([field: UProperty] UPokemon? Pokemon);

[UStruct]
public readonly partial record struct FSelectedItem([field: UProperty] Option<FItemSlotInfo> Item);

[USingleDelegate]
public delegate bool ItemFilter(FItemHandle item);

[UClass]
public class UPokemonAsyncBlueprintLibrary : UBlueprintFunctionLibrary
{
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Async")]
    public static Task ShowMessageAsync(
        APlayerController playerController,
        FText message,
        CancellationToken cancellationToken = default
    )
    {
        return playerController.ShowMessageAsync(message, cancellationToken);
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Async")]
    public static Task<int> ShowChoicePrompt(
        APlayerController playerController,
        FText message,
        IReadOnlyList<FText> choices,
        CancellationToken cancellationToken = default
    )
    {
        return playerController.ShowChoicePrompt(message, choices, cancellationToken);
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Async")]
    public static Task<bool> ShowYesNoPromptAsync(
        APlayerController playerController,
        FText message,
        FText yesText,
        FText noText,
        CancellationToken cancellationToken = default
    )
    {
        return playerController.ShowYesNoPromptAsync(message, yesText, noText, cancellationToken);
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Async")]
    public static Task<FSelectedPokemon> SelectPokemonFromPartyAsync(
        APlayerController playerController,
        CancellationToken cancellationToken = default
    )
    {
        return playerController
            .SelectPokemonFromPartyAsync(cancellationToken)
            .Map(p => new FSelectedPokemon(p));
    }

    [UFunction(
        FunctionFlags.BlueprintCallable,
        DisplayName = "Match (Selected Pokémon)",
        Category = "Async"
    )]
    [ExpandEnumAsExecs("ReturnValue")]
    public static ESelectionResult MatchSelectedPokemon(
        FSelectedPokemon selectedPokemon,
        out UPokemon? pokemon
    )
    {
        pokemon = selectedPokemon.Pokemon;
        return pokemon is not null ? ESelectionResult.Selected : ESelectionResult.Cancelled;
    }

    [UFunction(
        FunctionFlags.BlueprintCallable,
        DisplayName = "Match (Selected Pokémon)",
        Category = "Async"
    )]
    public static Task<FSelectedItem> SelectItemFromBagAsync(
        APlayerController playerController,
        FGameplayTag pocketFilter,
        TDelegate<ItemFilter> filter,
        CancellationToken cancellationToken = default
    )
    {
        return playerController
            .SelectItemFromBagAsync(
                pocketFilter.IsValid ? pocketFilter : null,
                i => !filter.IsBound || filter.InnerDelegate.Invoke(i),
                cancellationToken
            )
            .Map(i => new FSelectedItem(i.ToOption()));
    }

    [UFunction(
        FunctionFlags.BlueprintCallable,
        DisplayName = "Match (Selected Item)",
        Category = "Async"
    )]
    [ExpandEnumAsExecs("ReturnValue")]
    public static ESelectionResult MatchSelectedItem(
        FSelectedItem selectedItem,
        out FItemHandle item,
        out int quantity
    )
    {
        var slot = selectedItem.Item.ToNullable();
        if (slot.HasValue)
        {
            item = slot.Value.Item;
            quantity = slot.Value.Quantity;
            return ESelectionResult.Selected;
        }

        item = default;
        quantity = 0;
        return ESelectionResult.Cancelled;
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Async")]
    public static Task<bool> PromptReplaceMoveAsync(
        APlayerController playerController,
        UPokemon pokemon,
        FMoveHandle move,
        CancellationToken cancellationToken = default
    )
    {
        return playerController.PromptReplaceMoveAsync(pokemon, move, cancellationToken);
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Async")]
    public static Task<bool> GivePokemonItemFromBagAsync(
        APlayerController playerController,
        UPokemon pokemon,
        CancellationToken cancellationToken = default
    )
    {
        return playerController.GivePokemonItemFromBagAsync(pokemon, cancellationToken);
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Async")]
    public static Task<bool> GiveItemToPokemonAsync(
        APlayerController playerController,
        UPokemon pokemon,
        FItemHandle item,
        CancellationToken cancellationToken = default
    )
    {
        return playerController.GiveItemToPokemonAsync(pokemon, item, cancellationToken);
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Async")]
    public static Task<bool> TakeItemFromPokemonAsync(
        APlayerController playerController,
        UPokemon pokemon,
        CancellationToken cancellationToken = default
    )
    {
        return playerController.TakeItemFromPokemonAsync(pokemon, cancellationToken);
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Async")]
    public static Task<bool> LearnMoveAsync(
        APlayerController playerController,
        UPokemon pokemon,
        FMoveHandle move,
        CancellationToken cancellationToken = default
    )
    {
        return playerController.LearnMoveAsync(pokemon, move, cancellationToken);
    }
}
