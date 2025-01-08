// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/PokemonCoroutineDispatcherImpl.h"
#include "DataRetrieval/DataUtilities.h"
#include "Player/Bag.h"
#include "Pokemon/Pokemon.h"
#include "PokemonUISettings.h"
#include "RPGUIManagerSubsystem.h"
#include "Screens/Screen.h"
#include "Strings/StringUtilities.h"
#include "Utilities/PokemonUIAsyncActions.h"
#include "Utilities/TrainerHelpers.h"

UE5Coro::TCoroutine<bool> UPokemonCoroutineDispatcherImpl::GiveItemToPokemon(const UObject *WorldContext,
                                                                             const FItemHandle &Item,
                                                                             const TScriptInterface<IPokemon> Pokemon,
                                                                             int PokemonIndex) const {
    auto &Messages = GetDefault<UPokemonUISettings>()->ItemMessages;

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
        URPGUIManagerSubsystem::Get(WorldContext).GetTopScreenOfStack()->RefreshSelf();

        auto &NewItem = FDataManager::GetInstance().GetDataChecked(Item);
        co_await Pokemon::UI::DisplayMessage(
            WorldContext, FText::FormatNamed(Messages.SwappedItemsMessage, TEXT("Pkmn"), Pokemon->GetNickname(),
                                             TEXT("Old"), OldItemName, TEXT("New"), NewItem.GetPortionName()));
    } else {
        Pokemon->SetHoldItem(Item);
        UTrainerHelpers::GetBag(WorldContext)->RemoveItem(Item);
        URPGUIManagerSubsystem::Get(WorldContext).GetTopScreenOfStack()->RefreshSelf();

        auto &NewItem = FDataManager::GetInstance().GetDataChecked(Item);
        co_await Pokemon::UI::DisplayMessage(WorldContext, FText::FormatNamed(Messages.NowHoldingMessage, TEXT("Pkmn"),
                                                                              Pokemon->GetNickname(), TEXT("Item"),
                                                                              NewItem.GetPortionName()));
    }

    co_return true;
}