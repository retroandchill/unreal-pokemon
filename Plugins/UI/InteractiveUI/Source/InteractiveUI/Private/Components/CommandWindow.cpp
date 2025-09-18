// "Unreal Pokémon" created by Retro & Chill.
#include "Components/CommandWindow.h"
#include "Algo/ForEach.h"
#include "Blueprint/WidgetTree.h"
#include "Components/DisplayTextOption.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Data/Command.h"

UCommandWindow::UCommandWindow(const FObjectInitializer &ObjectInitializer) : USelectableWidget(ObjectInitializer)
{
}

void UCommandWindow::NativePreConstruct()
{
    Super::NativePreConstruct();
    AddCommands();
}

void UCommandWindow::SetCommands(TArray<TObjectPtr<UCommand>> &&NewCommands)
{
    Commands = std::move(NewCommands);
    AddCommands();
}

FProcessCommand &UCommandWindow::GetOnCommandSelected()
{
    return OnCommandSelected;
}

void UCommandWindow::NativeProcessConfirm(int32 CurrentIndex)
{
    if (CurrentIndex >= ActiveCommands.Num())
        return;

    UCommand *CurrentCommand = ActiveCommands[CurrentIndex];
    OnCommandSelected.Broadcast(CurrentIndex, CurrentCommand);
}

void UCommandWindow::AddCommands()
{
    ActiveCommands.Reset();
    if (DisplayTextWidgetClass == nullptr)
    {
        return;
    }

    ClearSelectableOptions();

    for (UCommand *const Command : Commands)
    {
        if (Command == nullptr || !Command->IsEnabled())
            continue;

        auto TextWidget = WidgetTree->ConstructWidget<UDisplayTextOption>(DisplayTextWidgetClass);
        TextWidget->SetText(Command->GetText());

        SlotOption(TextWidget);
        ActiveCommands.Add(Command);
    }
}
