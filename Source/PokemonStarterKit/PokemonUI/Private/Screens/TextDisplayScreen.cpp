// "Unreal Pokémon" created by Retro & Chill.
#include "Screens/TextDisplayScreen.h"
#include "Components/CommandWindow.h"
#include "Components/MessageWindow.h"
#include "Data/Command.h"
#include "Utilities/InteractiveUIUtilities.h"

DEFINE_INJECTABLE_DEPENDENCY(UTextDisplayScreen)

UTextDisplayScreen *UTextDisplayScreen::AddTextDisplayScreenToOverlay(const UObject *WorldContextObject)
{
    return UInteractiveUIUtilities::InjectScreenToOverlay<UTextDisplayScreen>(WorldContextObject).GetPtrOrNull();
}

void UTextDisplayScreen::NativeConstruct()
{
    Super::NativeConstruct();
    if (MessageWindow == nullptr)
        return;

    MessageWindow->GetOnDisplayChoices().AddUniqueDynamic(this,
                                                          &UTextDisplayScreen::UTextDisplayScreen::DisplayChoicePrompt);
    MessageWindow->GetOnAdvanceText().AddUniqueDynamic(this, &UTextDisplayScreen::AdvanceToNextMessage);
    MessageWindow->ActivateWidget();

    CommandWindow->GetOnCommandSelected().AddUniqueDynamic(this, &UTextDisplayScreen::ProcessSelectedChoice);
}

void UTextDisplayScreen::SetText(FText TextToDisplay)
{
    check(MessageWindow != nullptr)
    bAdvancedMessage = false;
    MessageWindow->ClearDisplayText();
    MessageWindow->SetDisplayText(TextToDisplay);
    CommandWindow->SetVisibility(ESlateVisibility::Collapsed);
    MessageWindow->ActivateWidget();
}

void UTextDisplayScreen::DisplayChoices(FText TextToDisplay, const TArray<FText> &Choices)
{
    check(MessageWindow != nullptr && CommandWindow != nullptr)
    bAdvancedMessage = false;
    MessageWindow->ClearDisplayText();
    MessageWindow->SetDisplayText(TextToDisplay, true);

    CommandWindow->SetVisibility(ESlateVisibility::Collapsed);
    TArray<TObjectPtr<UCommand>> Commands;
    for (const auto &Choice : Choices)
    {
        Commands.Add(UCommand::CreateBasicCommand(Choice));
    }
    CommandWindow->SetCommands(std::move(Commands));
    CommandWindow->ActivateWidget();
}

void UTextDisplayScreen::ClearDisplayText()
{
    check(MessageWindow != nullptr)
    MessageWindow->ClearDisplayText();
    CommandWindow->SetVisibility(ESlateVisibility::Collapsed);
}

void UTextDisplayScreen::AdvanceToNextMessage()
{
    if (bAdvancedMessage)
    {
        return;
    }

    bAdvancedMessage = true;
    CheckIfMoreTextIsAdded();
    NextMessage.Broadcast();
}

void UTextDisplayScreen::DisplayChoicePrompt()
{
    CommandWindow->SetIndex(0);
    CommandWindow->SetVisibility(ESlateVisibility::Visible);
    CommandWindow->ActivateWidget();
    MessageWindow->DeactivateWidget();
}

void UTextDisplayScreen::ProcessSelectedChoice(int32 Index, UCommand *Choice)
{
    ProcessChoice.Broadcast(Index, Choice->GetID());
    CommandWindow->DeactivateWidget();
}

UE5Coro::TCoroutine<UCommonActivatableWidget *> UTextDisplayScreen::AwaitInputPrompt(FForceLatentCoroutine) const
{
    co_await UE5Coro::Latent::Until([this] { return MessageWindow->IsAwaitingInput(); });
    if (CommandWindow->IsVisible())
    {
        co_return CommandWindow;
    }

    co_return MessageWindow;
}

UE5Coro::TCoroutine<> UTextDisplayScreen::CheckIfMoreTextIsAdded(FForceLatentCoroutine)
{
    co_await UE5Coro::Latent::NextTick();
    if (bAdvancedMessage)
    {
        CloseScreen();
    }
}