// "Unreal Pokémon" created by Retro & Chill.


#include "RPGMenusTestUtilities.h"
#include "CommonButtonBase.h"
#include "Input/UIActionBinding.h"
#include "Windows/MessageWindow.h"

void URPGMenusTestUtilities::SelectCurrentOption(USelectableWidget *SelectableWidget) {
    SelectableWidget->GetSelectedOption()->OnClicked().Broadcast();
}

void URPGMenusTestUtilities::CancelOnWidget(USelectableWidget *SelectableWidget) {
    SelectableWidget->GetOnCancel().Broadcast();
}

void URPGMenusTestUtilities::ForceAdvanceText(UMessageWindow *MessageWindow, FName InputName) {
    auto MenuActionHandle = MessageWindow->GetActionBindings().FindByPredicate([InputName](const FUIActionBindingHandle &Handle) {
        return Handle.GetActionName() == InputName;
    });
    if (MenuActionHandle == nullptr) {
        return;
    }

    auto MenuAction = FUIActionBinding::FindBinding(*MenuActionHandle);
    if (MenuAction == nullptr) {
        return;
    }

    MenuAction->OnExecuteAction.ExecuteIfBound();
}