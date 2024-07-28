// "Unreal Pokémon" created by Retro & Chill.
#include "Components/MessageWindow.h"
#include "CommonTextBlock.h"
#include "Components/DisplayText.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Fonts/FontMeasure.h"
#include "Input/CommonUIInputTypes.h"
#include "MathUtilities.h"
#include "Components/DialogueBox.h"
#include "Utilities/WidgetUtilities.h"

void UMessageWindow::NativeConstruct() {
    Super::NativeConstruct();
    FBindUIActionArgs BindArgs(AdvanceActionInput, FSimpleDelegate::CreateWeakLambda(this, [this] {
                                   SetPaused(false);
                                   if (DialogueBox->HasFinishedPlayingLine() && !bWaitForChoice) {
                                       OnAdvanceText.Broadcast();
                                   }
                               }));
    BindArgs.bDisplayInActionBar = false;
    AdvanceAction = RegisterUIActionBinding(BindArgs);

    using FLineFinishedDelegate = FOnLineFinishedPlaying::FDelegate;
    DialogueBox->BindToOnLineFinishedPlaying(FLineFinishedDelegate::CreateWeakLambda(this, [this] {
        if (DialogueBox->GetCurrentLine().IsEmpty()) {
            return;
        }
        
        SetPaused(true);

        if (bWaitForChoice) {
            OnDisplayChoices.Broadcast();
        }
    }));
}

void UMessageWindow::NativeDestruct() {
    if (AdvanceAction.IsValid()) {
        AdvanceAction.Unregister();
    }
    Super::NativeDestruct();
}

void UMessageWindow::SetDisplayText(FText Text, bool bHasCommands) {
    check(DialogueBox != nullptr)

    bWaitForChoice = bHasCommands;
    DialogueBox->PlayLine(Text);
}

void UMessageWindow::ClearDisplayText() {
    check(DialogueBox != nullptr)
    DialogueBox->PlayLine(FText::FromString(TEXT("")));
}

void UMessageWindow::SetPaused(bool bPausedIn) {
    bPaused = bPausedIn;
    AdvanceAction.SetDisplayInActionBar(bPaused);

    if (PauseArrow == nullptr)
        return;
    
    if (bPaused) {
        PauseArrow->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    } else {
        PauseArrow->SetVisibility(ESlateVisibility::Hidden);
    }
}

bool UMessageWindow::IsAwaitingInput() const {
    return bPaused;
}

FAdvanceText &UMessageWindow::GetOnAdvanceText() {
    return OnAdvanceText;
}

FDisplayChoices &UMessageWindow::GetOnDisplayChoices() {
    return OnDisplayChoices;
}
