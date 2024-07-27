// "Unreal Pokémon" created by Retro & Chill.
#include "Screens/TextDisplayScreen.h"
#include "Data/Command.h"
#include "Components/CommandWindow.h"
#include "Components/MessageWindow.h"

void UTextDisplayScreen::NativeConstruct() {
    Super::NativeConstruct();
    if (MessageWindow == nullptr)
        return;

    MessageWindow->GetOnDisplayChoices().AddUniqueDynamic(this,
                                                          &UTextDisplayScreen::UTextDisplayScreen::DisplayChoicePrompt);
    MessageWindow->GetOnAdvanceText().AddUniqueDynamic(this, &UTextDisplayScreen::AdvanceToNextMessage);
    MessageWindow->ActivateWidget();

    CommandWindow->GetOnCommandSelected().AddUniqueDynamic(this, &UTextDisplayScreen::ProcessSelectedChoice);
}

void UTextDisplayScreen::SetText(FText TextToDisplay) {
    check(MessageWindow != nullptr)
    MessageWindow->ClearDisplayText();
    MessageWindow->SetDisplayText(TextToDisplay);
    CommandWindow->SetVisibility(ESlateVisibility::Collapsed);
    MessageWindow->ActivateWidget();
}

void UTextDisplayScreen::DisplayChoices(FText TextToDisplay, const TArray<FText> &Choices) {
    check(MessageWindow != nullptr && CommandWindow != nullptr)
    MessageWindow->ClearDisplayText();
    MessageWindow->SetDisplayText(TextToDisplay, true);

    CommandWindow->SetVisibility(ESlateVisibility::Collapsed);
    TArray<TObjectPtr<UCommand>> Commands;
    for (const auto &Choice : Choices) {
        Commands.Add(UCommand::CreateBasicCommand(Choice));
    }
    CommandWindow->SetCommands(MoveTemp(Commands));
    MessageWindow->ActivateWidget();
}

void UTextDisplayScreen::ClearDisplayText() {
    check(MessageWindow != nullptr)
    MessageWindow->ClearDisplayText();
    CommandWindow->SetVisibility(ESlateVisibility::Collapsed);
}

void UTextDisplayScreen::AdvanceToNextMessage() {
    NextMessage.Broadcast();
}

void UTextDisplayScreen::DisplayChoicePrompt() {
    CommandWindow->SetIndex(0);
    CommandWindow->SetVisibility(ESlateVisibility::Visible);
    CommandWindow->ActivateWidget();
    MessageWindow->DeactivateWidget();
}

void UTextDisplayScreen::ProcessSelectedChoice(int32 Index, UCommand *Choice) {
    ProcessChoice.Broadcast(Index, Choice->GetID());
    CommandWindow->DeactivateWidget();
}
