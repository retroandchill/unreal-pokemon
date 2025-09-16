// "Unreal Pokémon" created by Retro & Chill.
#include "Components/MessageWindow.h"
#include "Components/DialogueBox.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Input/CommonUIInputTypes.h"
#include "RetroLib/Utils/Math.h"

void UMessageWindow::NativeConstruct()
{
    Super::NativeConstruct();
    FBindUIActionArgs BindArgs(AdvanceActionInput, FSimpleDelegate::CreateWeakLambda(this, [this] {
                                   SetPaused(false);
                                   if (DialogueBox->HasFinishedPlayingLine() && !bWaitForChoice)
                                   {
                                       OnAdvanceText.Broadcast();
                                   }
                               }));
    BindArgs.bDisplayInActionBar = false;
    AdvanceAction = RegisterUIActionBinding(BindArgs);
    
    DialogueBox->BindToOnLineFinishedPlaying(FSimpleDelegate::CreateWeakLambda(this, [this] {
        if (DialogueBox->GetCurrentLine().IsEmpty())
        {
            return;
        }

        SetPaused(true);

        if (bWaitForChoice)
        {
            OnDisplayChoices.Broadcast();
        }
    }));
}

void UMessageWindow::NativeDestruct()
{
    if (AdvanceAction.IsValid())
    {
        AdvanceAction.Unregister();
    }
    Super::NativeDestruct();
}

void UMessageWindow::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (float BottomScroll = ScrollBox->GetScrollOffsetOfEnd(); ScrollTimer.IsSet() && OriginalScroll.IsSet())
    {
        ScrollTimer.GetValue() += InDeltaTime;
        ScrollBox->SetScrollOffset(
            Retro::LinearInterpolation(OriginalScroll.GetValue(), BottomScroll, ScrollSpeed, ScrollTimer.GetValue()));

        if (FMath::IsNearlyEqual(ScrollBox->GetScrollOffset(), BottomScroll))
        {
            ScrollTimer.Reset();
            OriginalScroll.Reset();
        }
    }
    else if (float CurrentScroll = ScrollBox->GetScrollOffset(); !FMath::IsNearlyEqual(CurrentScroll, BottomScroll))
    {
        ScrollTimer.Emplace(0.f);
        OriginalScroll.Emplace(CurrentScroll);
    }
}

const FText &UMessageWindow::GetDisplayText() const
{
    return DialogueBox->GetCurrentLine();
}

void UMessageWindow::SetDisplayText(FText Text, bool bHasCommands)
{
    check(DialogueBox != nullptr)

    bWaitForChoice = bHasCommands;
    DialogueBox->PlayLine(Text);
}

void UMessageWindow::ClearDisplayText()
{
    check(DialogueBox != nullptr)
    DialogueBox->PlayLine(FText::FromString(TEXT("")));
}

void UMessageWindow::SetPaused(bool bPausedIn)
{
    bPaused = bPausedIn;
    AdvanceAction.SetDisplayInActionBar(bPaused);

    if (PauseArrow == nullptr)
        return;

    if (bPaused)
    {
        PauseArrow->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
    else
    {
        PauseArrow->SetVisibility(ESlateVisibility::Hidden);
    }
}

bool UMessageWindow::IsAwaitingInput() const
{
    return bPaused;
}

FAdvanceText &UMessageWindow::GetOnAdvanceText()
{
    return OnAdvanceText;
}

FDisplayChoices &UMessageWindow::GetOnDisplayChoices()
{
    return OnDisplayChoices;
}
