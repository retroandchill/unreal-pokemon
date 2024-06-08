// "Unreal Pokémon" created by Retro & Chill.
#include "Screens/PokemonSelectScreen.h"
#include "Data/Command.h"
#include "Handlers/PartyMenu/PartyMenuHandler.h"
#include "Handlers/PartyMenu/PartyMenuHandlerSet.h"
#include "Managers/PokemonSubsystem.h"
#include "Utilities/PokemonUIUtils.h"
#include "Windows/CommandWindow.h"
#include "Windows/HelpWindow.h"
#include "Windows/PokemonSelectionPane.h"

void UPokemonSelectScreen::NativeConstruct() {
    Super::NativeConstruct();
    check(SelectionPane != nullptr)
    SelectionPane->SetIndex(0);
    SelectionPane->SetActive(true);
    SelectionPane->GetOnConfirm().AddDynamic(this, &UPokemonSelectScreen::OnPokemonSelected);
    SelectionPane->GetOnCancel().AddDynamic(this, &UPokemonSelectScreen::CloseScreen);
    CommandWindow->GetOnCommandSelected().AddDynamic(this, &UPokemonSelectScreen::ProcessCommand);
    CommandWindow->GetOnCancel().AddDynamic(this, &UPokemonSelectScreen::OnCommandWindowCancel);
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

void UPokemonSelectScreen::ShowCommands(const TArray<TObjectPtr<UPartyMenuHandler>> &Handlers) {
    if (!CommandStack.IsEmpty()) {
        CommandStack.Last().Index = SelectionPane->GetIndex();
    }

    auto Trainer = UPokemonSubsystem::GetInstance(this).GetPlayer();
    auto &[Commands, FrameIndex] = CommandStack.AddDefaulted_GetRef();
    Commands =
        UPokemonUIUtils::CreateCommandListFromHandlers(Handlers, CancelText, this, Trainer, SelectionPane->GetIndex());
    CommandWindow->SetCommands(Commands);
    CommandWindow->SetIndex(0);
}

void UPokemonSelectScreen::ClearCommandStack() {
    CommandStack.Empty();
    ToggleCommandWindowVisibility(false);
    SelectionPane->ToggleCommandVisibility(true);
    CommandWindow->SetActive(false);
    SelectionPane->SetActive(true);
}

void UPokemonSelectScreen::SetCommandHelpText(FText Text) {
    CommandHelpWindow->SetText(Text);
}

APlayerController *UPokemonSelectScreen::GetPlayerController() const {
    return GetOwningPlayer();
}

FOnPokemonSelected &UPokemonSelectScreen::GetOnPokemonSelect() {
    return PokemonSelected;
}

void UPokemonSelectScreen::RefreshScene() {
    SelectionPane->RefreshWindow();
}

void UPokemonSelectScreen::RefreshSelf_Implementation() {
    Super::RefreshSelf_Implementation();
    RefreshScene();
}

void UPokemonSelectScreen::SetHelpText(FText Text) {
    HelpWindow->SetText(Text);
}

void UPokemonSelectScreen::RemoveFromStack() {
    CloseScreen();
}

void UPokemonSelectScreen::OnPokemonSelected(int32 Index) {
    auto Trainer = UPokemonSubsystem::GetInstance(this).GetPlayer();
    if (PokemonSelected.IsBound()) {
        PokemonSelected.Execute(this, Trainer, Index);
        return;
    }

    if (SelectionPane->IsSwitching()) {
        if (int32 SwitchingIndex = SelectionPane->GetSwitchingIndex().GetValue(); Index != SwitchingIndex) {
            Trainer->SwapPositionsInParty(SwitchingIndex, Index);
        }
        SelectionPane->CompleteSwitch();
    } else {
        DisplayPokemonCommands(Trainer, Index);
    }
}

void UPokemonSelectScreen::DisplayPokemonCommands(const TScriptInterface<ITrainer> &Trainer, int32 Index) {
    auto &[Commands, FrameIndex] = CommandStack.AddDefaulted_GetRef();
    Commands = UPokemonUIUtils::CreateCommandListFromHandlers(PokemonHandlers->GetHandlers(), CancelText, this, Trainer, Index);
    CommandWindow->SetCommands(Commands);

    SelectionPane->SetActive(false);
    CommandWindow->SetIndex(0);
    CommandWindow->SetActive(true);

    SelectionPane->ToggleCommandVisibility(false);
    ToggleCommandWindowVisibility(true);
}

void UPokemonSelectScreen::ProcessCommand(int32, UCommand *SelectedCommand) {
    static FName Cancel = "Cancel";
    if (SelectedCommand->GetID() == Cancel) {
        OnCommandWindowCancel();
    } else {
        auto Handler = SelectedCommand->GetHandler<UPartyMenuHandler>();
        check(Handler != nullptr)
        Handler->Handle(this, UPokemonSubsystem::GetInstance(this).GetPlayer(), SelectionPane->GetIndex());
    }
}

void UPokemonSelectScreen::OnCommandWindowCancel() {
    CommandStack.Pop();
    if (CommandStack.IsEmpty()) {
        ToggleCommandWindowVisibility(false);
        SelectionPane->ToggleCommandVisibility(true);
        CommandWindow->SetActive(false);
        SelectionPane->SetActive(true);
    } else {
        auto &[Commands, FrameIndex] = CommandStack.Last();
        CommandWindow->SetCommands(Commands);
        CommandWindow->SetIndex(FrameIndex);
    }
}

void UPokemonSelectScreen::ToggleCommandWindowVisibility(bool bIsVisible) {
    using enum ESlateVisibility;
    if (bIsVisible) {
        CommandWindow->SetVisibility(Visible);
        CommandHelpWindow->SetVisibility(SelfHitTestInvisible);
    } else {
        CommandWindow->SetVisibility(Collapsed);
        CommandHelpWindow->SetVisibility(Collapsed);
    }
}