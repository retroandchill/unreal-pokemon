// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/PokemonCoroutineDispatcherImpl.h"
#include "DataRetrieval/DataUtilities.h"
#include "GameUIManagerSubsystem.h"
#include "Player/Bag.h"
#include "Pokemon/Moves/MoveBlock.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"
#include "PokemonDataSettings.h"
#include "PrimaryGameLayout.h"
#include "RetroLib/Optionals/IfPresent.h"
#include "RetroLib/Utils/StringUtilities.h"
#include "Screens/MoveForgetScreen.h"
#include "Screens/Screen.h"
#include "Screens/TextDisplayScreen.h"
#include "Settings/PokemonMessageSettings.h"
#include "Utilities/TrainerHelpers.h"

UE5Coro::TCoroutine<> UPokemonCoroutineDispatcherImpl::DisplayMessage(FText Message, FForceLatentCoroutine) const
{
    if (auto Controller = GetWorld()->GetFirstPlayerController(); Controller == nullptr)
    {
        // TODO: Remove this hack and alter how the tests run to avoid this
        co_return;
    }

    auto Layout = UPrimaryGameLayout::GetInstance(this);
    if (Layout == nullptr)
    {
        co_return;
    }
    auto Screen = Cast<UTextDisplayScreen>(Layout->GetLayerWidget(RPG::Menus::OverlayMenuLayerTag)->GetActiveWidget());
    if (Screen == nullptr)
    {
        Screen = UTextDisplayScreen::AddTextDisplayScreenToOverlay(this);
    }
    Screen->SetText(std::move(Message));
    co_await Screen->NextMessage;
}

IPokemonCoroutineDispatcher::TMultiCoroutine<int, FName> UPokemonCoroutineDispatcherImpl::DisplayMessageWithChoices(
    FText Message, const TArray<FText> &Choices, FForceLatentCoroutine) const
{
    auto Layout = UPrimaryGameLayout::GetInstance(this);
    auto Screen = Cast<UTextDisplayScreen>(Layout->GetLayerWidget(RPG::Menus::OverlayMenuLayerTag)->GetActiveWidget());
    if (Screen == nullptr)
    {
        Screen = UTextDisplayScreen::AddTextDisplayScreenToOverlay(this);
    }
    Screen->DisplayChoices(Message, Choices);
    auto [ChoiceIndex, ChoiceID] = co_await Screen->ProcessChoice;
    co_return {ChoiceIndex, ChoiceID};
}

UE5Coro::TCoroutine<bool> UPokemonCoroutineDispatcherImpl::DisplayConfirmPrompt(FText Message, FText ConfirmOption,
                                                                                FText CancelOption,
                                                                                FForceLatentCoroutine) const
{
    auto [ChoiceIndex, _] =
        co_await DisplayMessageWithChoices(std::move(Message), {std::move(ConfirmOption), std::move(CancelOption)});
    co_return ChoiceIndex == 0;
}

UE5Coro::TCoroutine<TOptional<FSelectedPokemonHandle>> UPokemonCoroutineDispatcherImpl::SelectPokemonFromParty(
    FForceLatentCoroutine) const
{
    /*
    auto Screen = UPokemonSelectScreen::AddPokemonSelectScreenToStack(this);
    co_return co_await Screen->PromptPokemonSelection();
    */
    co_return TOptional<FSelectedPokemonHandle>();
}

UE5Coro::TCoroutine<TOptional<FSelectedItemHandle>> UPokemonCoroutineDispatcherImpl::SelectItemFromBag(
    const FItemFilter &Filter, FForceLatentCoroutine) const
{
    /*
    auto Screen = UBagScreen::AddBagScreenToStack(this);
    Screen->ApplyItemFilter(Filter);
    co_return co_await Screen->PromptItemSelection();
    */
    co_return TOptional<FSelectedItemHandle>();
}

UE5Coro::TCoroutine<bool> UPokemonCoroutineDispatcherImpl::PromptReplaceMove(const TScriptInterface<IPokemon> &Pokemon,
                                                                             FMoveHandle Move,
                                                                             FForceLatentCoroutine) const
{
    auto Screen = UMoveForgetScreen::AddMoveForgetScreenToStack(this);
    Screen->InitializeScene(Pokemon, Move);
    co_return co_await Screen->AwaitPlayerDecision();
}

UE5Coro::TCoroutine<bool> UPokemonCoroutineDispatcherImpl::GiveItemToPokemon(const FItemHandle &Item,
                                                                             const TScriptInterface<IPokemon> Pokemon,
                                                                             int PokemonIndex,
                                                                             FForceLatentCoroutine) const
{
    auto &Messages = *GetDefault<UPokemonMessageSettings>();

    if (auto HeldItem = Pokemon->GetHoldItem(); HeldItem.IsSet())
    {
        auto &OldItemName = HeldItem->GetPortionName();
        auto &TextFormat = UStringUtilities::StartsWithVowelText(OldItemName) ? Messages.AlreadyHoldingItemConsonant
                                                                              : Messages.AlreadyHoldingItemVowel;
        co_await DisplayMessage(FText::FormatNamed(TextFormat, "Pkmn", Pokemon->GetNickname(), "Item", OldItemName));
        if (!co_await DisplayConfirmPrompt(Messages.SwitchItemPrompt))
        {
            co_return false;
        }

        auto &Bag = UTrainerHelpers::GetBag(this);
        Pokemon->SetHoldItem(Item);
        Bag->RemoveItem(Item);
        Bag->ObtainItem(HeldItem->ID);

        auto &NewItem = FDataManager::GetInstance().GetDataChecked(Item);
        co_await DisplayMessage(FText::FormatNamed(Messages.SwappedItemsMessage, TEXT("Pkmn"), Pokemon->GetNickname(),
                                                   TEXT("Original"), OldItemName, TEXT("New"),
                                                   NewItem.GetPortionName()));
    }
    else
    {
        Pokemon->SetHoldItem(Item);
        UTrainerHelpers::GetBag(this)->RemoveItem(Item);

        auto &NewItem = FDataManager::GetInstance().GetDataChecked(Item);
        co_await DisplayMessage(FText::FormatNamed(Messages.NowHoldingMessage, TEXT("Pkmn"), Pokemon->GetNickname(),
                                                   TEXT("Item"), NewItem.GetPortionName()));
    }

    co_return true;
}

UE5Coro::TCoroutine<bool> UPokemonCoroutineDispatcherImpl::TakeItemFromPokemon(
    const TScriptInterface<IPokemon> &Pokemon, FForceLatentCoroutine) const
{
    auto &Messages = *GetDefault<UPokemonMessageSettings>();

    auto WorldContext = Pokemon.GetObject();
    auto HeldItem = Pokemon->GetHoldItem();
    auto Bag = UTrainerHelpers::GetBag(WorldContext);
    if (!HeldItem.IsSet() || !Bag->CanObtainItem(HeldItem->ID))
    {
        co_await DisplayMessage(Messages.CannotTakeItemMessage);
        co_return false;
    }

    Bag->ObtainItem(HeldItem->ID);
    Pokemon->RemoveHoldItem();

    co_await DisplayMessage(FText::FormatNamed(Messages.TookItemMessage, TEXT("Pkmn"), Pokemon->GetNickname(),
                                               TEXT("Item"), HeldItem->GetPortionName()));
    co_return true;
}

UE5Coro::TCoroutine<bool> UPokemonCoroutineDispatcherImpl::LearnMove(const TScriptInterface<IPokemon> &Pokemon,
                                                                     FMoveHandle Move, FForceLatentCoroutine) const
{
    auto &Messages = *GetDefault<UPokemonMessageSettings>();
    auto &MoveData = FDataManager::GetInstance().GetDataChecked(Move);
    auto Nickname = Pokemon->GetNickname();

    auto MoveBlock = Pokemon->GetMoveBlock();
    if (MoveBlock->HasOpenMoveSlot())
    {
        MoveBlock->PlaceMoveInOpenSlot(Move);
        co_return true;
    }

    auto WorldContext = Pokemon.GetObject();
    co_await DisplayMessage(FText::FormatNamed(Messages.LearnedMoveMessage, TEXT("Pkmn"), Nickname, TEXT("Move"),
                                               MoveData.RealName, TEXT("Count"),
                                               GetDefault<UPokemonDataSettings>()->MaxMoves));
    if (!co_await DisplayConfirmPrompt(
            FText::FormatNamed(Messages.ForgetMovePrompt, TEXT("Pkmn"), Nickname, TEXT("Move"), MoveData.RealName)))
    {
        co_await DisplayMessage(
            FText::FormatNamed(Messages.LearnedMoveMessage, TEXT("Pkmn"), Nickname, TEXT("Move"), MoveData.RealName));
        co_return false;
    }

    co_return co_await PromptReplaceMove(Pokemon, Move);
}

UE5Coro::TCoroutine<> UPokemonCoroutineDispatcherImpl::ProcessLevelUp(const TScriptInterface<IPokemon> &Pokemon,
                                                                      const FLevelUpStatChanges &StatChanges,
                                                                      FForceLatentCoroutine) const
{
    auto &Messages = *GetDefault<UPokemonMessageSettings>();
    auto Nickname = Pokemon->GetNickname();

    auto WorldContext = Pokemon.GetObject();
    auto [Before, After] = StatChanges.LevelChange;
    co_await DisplayMessage(FText::FormatNamed(Messages.GrewToLevelMessage, TEXT("Pkmn"), Nickname, TEXT("Level"),
                                               StatChanges.LevelChange.After));

    for (auto Move : Pokemon->GetMoveBlock()->GetLevelUpMoves(Before, After))
    {
        co_await LearnMove(Pokemon, Move);
    }
}