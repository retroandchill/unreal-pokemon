// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/PokemonCoroutineDispatcherImpl.h"
#include "PokemonDataSettings.h"
#include "DataRetrieval/DataUtilities.h"
#include "Player/Bag.h"
#include "Pokemon/Pokemon.h"
#include "RetroLib/Optionals/IfPresent.h"
#include "RetroLib/Utils/StringUtilities.h"
#include "RPGUIManagerSubsystem.h"
#include "Pokemon/Moves/MoveBlock.h"
#include "Screens/Screen.h"
#include "Settings/PokemonMessageSettings.h"
#include "Utilities/PokemonUIAsyncActions.h"
#include "Utilities/TrainerHelpers.h"

UE5Coro::TCoroutine<bool> UPokemonCoroutineDispatcherImpl::GiveItemToPokemon(const UObject *WorldContext,
                                                                             const FItemHandle &Item,
                                                                             const TScriptInterface<IPokemon> Pokemon,
                                                                             int PokemonIndex, FForceLatentCoroutine Coro) const {
    auto &Messages = *GetDefault<UPokemonMessageSettings>();

    auto HeldItem = Pokemon->GetHoldItem();
    if (HeldItem.IsSet()) {
        auto &OldItemName = HeldItem->GetPortionName();
        auto &TextFormat = UStringUtilities::StartsWithVowelText(OldItemName) ? Messages.AlreadyHoldingItemConsonant
                                                                              : Messages.AlreadyHoldingItemVowel;
        co_await Pokemon::UI::DisplayMessage(
            WorldContext, FText::FormatNamed(TextFormat, "Pkmn", Pokemon->GetNickname(), "Item", OldItemName));
        if (!co_await Pokemon::UI::DisplayConfirmPrompt(WorldContext, Messages.SwitchItemPrompt)) {
            co_return false;
        }

        auto &Bag = UTrainerHelpers::GetBag(WorldContext);
        Pokemon->SetHoldItem(Item);
        Bag->RemoveItem(Item);
        Bag->ObtainItem(HeldItem->ID);
        Retro::Optionals::OfNullable(URPGUIManagerSubsystem::Get(WorldContext).GetTopScreenOfStack()) |
            Retro::Optionals::IfPresent(&UScreen::RefreshSelf);

        auto &NewItem = FDataManager::GetInstance().GetDataChecked(Item);
        co_await Pokemon::UI::DisplayMessage(
            WorldContext, FText::FormatNamed(Messages.SwappedItemsMessage, TEXT("Pkmn"), Pokemon->GetNickname(),
                                             TEXT("Original"), OldItemName, TEXT("New"), NewItem.GetPortionName()));
    } else {
        Pokemon->SetHoldItem(Item);
        UTrainerHelpers::GetBag(WorldContext)->RemoveItem(Item);
        Retro::Optionals::OfNullable(URPGUIManagerSubsystem::Get(WorldContext).GetTopScreenOfStack()) |
            Retro::Optionals::IfPresent(&UScreen::RefreshSelf);

        auto &NewItem = FDataManager::GetInstance().GetDataChecked(Item);
        co_await Pokemon::UI::DisplayMessage(WorldContext, FText::FormatNamed(Messages.NowHoldingMessage, TEXT("Pkmn"),
                                                                              Pokemon->GetNickname(), TEXT("Item"),
                                                                              NewItem.GetPortionName()));
    }

    co_return true;
}

UE5Coro::TCoroutine<bool> UPokemonCoroutineDispatcherImpl::TakeItemFromPokemon(const UObject *WorldContext,
                                                                               const TScriptInterface<IPokemon> &Pokemon, FForceLatentCoroutine Coro) const {
    auto &Messages = *GetDefault<UPokemonMessageSettings>();
    
    auto HeldItem = Pokemon->GetHoldItem();
    auto Bag = UTrainerHelpers::GetBag(WorldContext);
    if (!HeldItem.IsSet() || !Bag->CanObtainItem(HeldItem->ID)) {
        co_await Pokemon::UI::DisplayMessage(WorldContext, Messages.CannotTakeItemMessage);
        co_return false;
    }
    
    Bag->ObtainItem(HeldItem->ID);
    Pokemon->RemoveHoldItem();
    Retro::Optionals::OfNullable(URPGUIManagerSubsystem::Get(WorldContext).GetTopScreenOfStack()) |
        Retro::Optionals::IfPresent(&UScreen::RefreshSelf);

    co_await Pokemon::UI::DisplayMessage(WorldContext, FText::FormatNamed(Messages.TookItemMessage, TEXT("Pkmn"), Pokemon->GetNickname(), TEXT("Item"), HeldItem->GetPortionName()));
    co_return true;
}

UE5Coro::TCoroutine<bool> UPokemonCoroutineDispatcherImpl::LearnMove(const UObject *WorldContext,
                                                                     const TScriptInterface<IPokemon> &Pokemon, FMoveHandle Move, FForceLatentCoroutine Coro) const {
    auto &Messages = *GetDefault<UPokemonMessageSettings>();
    auto &MoveData = FDataManager::GetInstance().GetDataChecked(Move);
    auto Nickname = Pokemon->GetNickname();
    
    auto MoveBlock = Pokemon->GetMoveBlock();
    if (MoveBlock->HasOpenMoveSlot()) {
        MoveBlock->PlaceMoveInOpenSlot(Move);
        co_return true;
    }
    
    co_await Pokemon::UI::DisplayMessage(WorldContext, FText::FormatNamed(Messages.LearnedMoveMessage, TEXT("Pkmn"), Nickname, TEXT("Move"), MoveData.RealName, TEXT("Count"), GetDefault<UPokemonDataSettings>()->MaxMoves));
    if (!co_await Pokemon::UI::DisplayConfirmPrompt(WorldContext, FText::FormatNamed(Messages.ForgetMovePrompt, TEXT("Pkmn"), Nickname, TEXT("Move"), MoveData.RealName))) {
        co_await Pokemon::UI::DisplayMessage(WorldContext, FText::FormatNamed(Messages.LearnedMoveMessage, TEXT("Pkmn"), Nickname, TEXT("Move"), MoveData.RealName));
        co_return false;
    }

    co_return co_await Pokemon::UI::PromptReplaceMove(WorldContext, Pokemon, Move);
}