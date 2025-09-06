// "Unreal Pokémon" created by Retro & Chill.

#include "RPGMenusTestUtilities.h"
#include "CommonButtonBase.h"
#include "Components/MessageWindow.h"
#include "Input/UIActionBinding.h"

void URPGMenusTestUtilities::SelectCurrentOption(USelectableWidget *SelectableWidget)
{
    if (!SelectableWidget->IsActivated())
    {
        UE_LOG(LogBlueprint, Error, TEXT("The window isn't activated"))
        return;
    }

    auto SelectedOption = SelectableWidget->GetSelectedOption();
    if (auto Function = SelectedOption->FindFunction("HandleButtonClicked"); Function == nullptr)
    {
        UE_LOG(LogBlueprint, Error, TEXT("Could not find function 'HandleButtonClicked'"))
        return;
    }

    FSimpleDelegate Delegate;
    Delegate.BindUFunction(SelectedOption, "HandleButtonClicked");
    Delegate.Execute();
}

void URPGMenusTestUtilities::CancelOnWidget(USelectableWidget *SelectableWidget)
{
    SelectableWidget->GetOnCancel().Broadcast();
}

void URPGMenusTestUtilities::ForceAdvanceText(UMessageWindow *MessageWindow, FName InputName)
{
    auto MenuActionHandle = MessageWindow->GetActionBindings().FindByPredicate(
        [InputName](const FUIActionBindingHandle &Handle) { return Handle.GetActionName() == InputName; });
    if (MenuActionHandle == nullptr)
    {
        return;
    }

    auto MenuAction = FUIActionBinding::FindBinding(*MenuActionHandle);
    if (MenuAction == nullptr)
    {
        return;
    }

    MenuAction->OnExecuteAction.ExecuteIfBound();
}

void URPGMenusTestUtilities::ForceClickButton(UCommonButtonBase *Button)
{
    Button->OnClicked().Broadcast();
}