// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/PokemonUIAsyncActions.h"
#include "PrimaryGameLayout.h"
#include "Screens/BagScreen.h"
#include "Screens/PokemonSelectScreen.h"
#include "Screens/TextDisplayScreen.h"

namespace Pokemon::UI {
    UE5Coro::TCoroutine<> Pokemon::UI::DisplayMessage(const UObject *WorldContext, FText Message) {
        if (auto Controller = WorldContext->GetWorld()->GetFirstPlayerController(); Controller == nullptr) {
            // TODO: Remove this hack and alter how the tests run to avoid this
            co_return;
        }

        auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContext);
        auto Screen = Cast<UTextDisplayScreen>(Layout->GetLayerWidget(RPG::Menus::OverlayMenuLayerTag)->GetActiveWidget());
        if (Screen == nullptr) {
            Screen = UTextDisplayScreen::AddTextDisplayScreenToOverlay(WorldContext);
        }
        Screen->SetText(std::move(Message));
        co_await Screen->NextMessage;
    }

    TMultiCoroutine<int32, FName> DisplayMessageWithChoices(const UObject *WorldContext, FText Message,
        const TArray<FText> &Choices) {
        auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContext);
        auto Screen = Cast<UTextDisplayScreen>(Layout->GetLayerWidget(RPG::Menus::OverlayMenuLayerTag)->GetActiveWidget());
        if (Screen == nullptr) {
            Screen = UTextDisplayScreen::AddTextDisplayScreenToOverlay(WorldContext);
        }
        Screen->DisplayChoices(Message, Choices);
        auto [ChoiceIndex, ChoiceID] = co_await Screen->ProcessChoice;
        co_return {ChoiceIndex, ChoiceID};
    }

    UE5Coro::TCoroutine<bool> DisplayConfirmPrompt(const UObject *WorldContext, FText Message, FText ConfirmOption,
        FText CancelOption) {
        auto [ChoiceIndex, _] = co_await DisplayMessageWithChoices(WorldContext, Message, {ConfirmOption, CancelOption});
        co_return ChoiceIndex == 0;
    }

    UE5Coro::TCoroutine<TOptional<FSelectedPokemonHandle>> SelectPokemonFromParty(const UObject *WorldContext) {
        auto Screen = UPokemonSelectScreen::AddPokemonSelectScreenToStack(WorldContext);
        co_return co_await Screen->PromptPokemonSelection();
    }

    UE5Coro::TCoroutine<TOptional<FSelectedItemHandle>> SelectItemFromBag(const UObject *WorldContext,
                                                                          const FItemFilter &Filter) {
        auto Screen = UBagScreen::AddBagScreenToStack(WorldContext);
        Screen->ApplyItemFilter(Filter);
        co_return co_await Screen->PromptItemSelection();
    }
}