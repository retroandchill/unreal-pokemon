// "Unreal Pokémon" created by Retro & Chill.
#include "Windows/CommandWindow.h"
#include "Algo/ForEach.h"
#include "Blueprint/WidgetTree.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Data/Command.h"
#include "Primatives/DisplayText.h"
#include "Primatives/DisplayTextOption.h"

UCommandWindow::UCommandWindow(const FObjectInitializer &ObjectInitializer) : USelectableWidget(ObjectInitializer) {
}

void UCommandWindow::NativePreConstruct() {
    Super::NativePreConstruct();
    AddCommands();
}

void UCommandWindow::SetCommands(const TArray<TObjectPtr<UCommand>> &NewCommands) {
    Commands = NewCommands;
    AddCommands();
}

void UCommandWindow::SetCommands(TArray<TObjectPtr<UCommand>> &&NewCommands) {
    Commands = MoveTemp(NewCommands);
    AddCommands();
}

FProcessCommand &UCommandWindow::GetOnCommandSelected() {
    return OnCommandSelected;
}

void UCommandWindow::ProcessConfirm_Implementation(int32 CurrentIndex) {
    if (CurrentIndex >= ActiveCommands.Num())
        return;

    UCommand *CurrentCommand = ActiveCommands[CurrentIndex];
    OnCommandSelected.Broadcast(CurrentIndex, CurrentCommand);
}

void UCommandWindow::AddCommands() {
    ActiveCommands.Reset();
    if (DisplayTextWidgetClass == nullptr) {
        return;
    }

    ClearSelectableOptions();

    for (UCommand *const Command : Commands) {
        if (Command == nullptr || !Command->IsEnabled())
            continue;

        auto TextWidget = WidgetTree->ConstructWidget<UDisplayTextOption>(DisplayTextWidgetClass);
        TextWidget->SetText(Command->GetText());

        int32 CurrentIndex = ActiveCommands.Num();
        SlotOption(TextWidget, CurrentIndex);
        ActiveCommands.Add(Command);
    }
}
