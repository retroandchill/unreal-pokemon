// "Unreal Pokémon" created by Retro & Chill.
#include "Screens/PokemonSelectScreen.h"

#include "Data/Command.h"
#include "Handlers/PartyMenu/PartyMenuHandler.h"
#include "Managers/PokemonSubsystem.h"
#include "RPGPlayerController.h"
#include "Windows/CommandWindow.h"
#include "Windows/HelpWindow.h"
#include "Windows/PokemonSelectionPane.h"

void UPokemonSelectScreen::NativeConstruct() {
    Super::NativeConstruct();
    check(SelectionPane != nullptr) SelectionPane->SetIndex(0);
    SelectionPane->SetActive(true);
    SelectionPane->OnConfirm.AddDynamic(this, &UPokemonSelectScreen::OnPokemonSelected);
    SelectionPane->OnCancel.AddDynamic(this, &UPokemonSelectScreen::CloseScreen);
    CommandWindow->OnCommandSelected.AddDynamic(this, &UPokemonSelectScreen::ProcessCommand);
    CommandWindow->OnCancel.AddDynamic(this, &UPokemonSelectScreen::OnCommandWindowCancel);
    ToggleCommandWindowVisibility(false);
}

void UPokemonSelectScreen::BeginSwitch(int32 Index) {
    CommandWindow->SetActive(false);
    ToggleCommandWindowVisibility(false);
    SelectionPane->ToggleCommandVisibility(true);

    SelectionPane->SetIndex(Index);
    SelectionPane->BeginSwitch(Index);
    SelectionPane->SetActive(true);
}

void UPokemonSelectScreen::SetHelpText(const FText &Text) { CommandHelpWindow->SetText(Text); }

void UPokemonSelectScreen::OnPokemonSelected(int32 Index) {
    if (auto &Trainer = *UPokemonSubsystem::GetInstance().GetPlayer(); Index < Trainer.GetParty().Num()) {
        if (SelectionPane->IsSwitching()) {
            if (int32 SwitchingIndex = SelectionPane->GetSwitchingIndex().GetValue(); Index != SwitchingIndex) {
                Trainer.SwapPositionsInParty(SwitchingIndex, Index);
            }
            SelectionPane->CompleteSwitch();
        } else {
            TArray<TObjectPtr<UCommand>> Commands;
            for (UPartyMenuHandler *Handler : PokemonHandlers) {
                if (!Handler->ShouldShow(*this, Trainer, Index))
                    continue;

                Commands.Add(UCommand::CreateBasicCommand(Handler->GetID(), Handler->GetText(), Handler));
            }
            Commands.Add(UCommand::CreateBasicCommand(TEXT("Cancel"), CancelText));
            CommandWindow->SetCommands(MoveTemp(Commands));

            SelectionPane->SetActive(false);
            CommandWindow->SetIndex(0);
            CommandWindow->SetActive(true);

            SelectionPane->ToggleCommandVisibility(false);
            ToggleCommandWindowVisibility(true);
        }
    } else {
        // TODO: Handle the additional options
    }
}

void UPokemonSelectScreen::ProcessCommand(int32 CurrentIndex, UCommand *SelectedCommand) {
    auto Handler = SelectedCommand->GetHandler<UPartyMenuHandler>();
    check(Handler != nullptr);
    Handler->Handle(*this, *UPokemonSubsystem::GetInstance().GetPlayer(), SelectionPane->GetIndex());
}

void UPokemonSelectScreen::OnCommandWindowCancel() {
    ToggleCommandWindowVisibility(false);
    SelectionPane->ToggleCommandVisibility(true);
    CommandWindow->SetActive(false);
    SelectionPane->SetActive(true);
}

void UPokemonSelectScreen::ToggleCommandWindowVisibility(bool bIsVisible) {
    if (bIsVisible) {
        CommandWindow->SetVisibility(ESlateVisibility::Visible);
        CommandHelpWindow->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    } else {
        CommandWindow->SetVisibility(ESlateVisibility::Collapsed);
        CommandHelpWindow->SetVisibility(ESlateVisibility::Collapsed);
    }
}
