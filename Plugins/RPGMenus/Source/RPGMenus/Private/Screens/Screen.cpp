// "Unreal Pokémon" created by Retro & Chill.
#include "Screens/Screen.h"
#include "Blueprint/WidgetTree.h"
#include "RPGMenusSubsystem.h"
#include "Windows/SelectableWidget.h"

UScreen::UScreen(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer) {
    SetIsFocusable(true);
}

TSharedRef<SWidget> UScreen::RebuildWidget() {
    auto Ret = Super::RebuildWidget();

    SelectableWidgets.Empty();
    WidgetTree->ForEachWidget([this](UWidget *Widget) {
        auto SelectableWidget = Cast<USelectableWidget>(Widget);
        if (SelectableWidget == nullptr)
            return;

        SelectableWidgets.Emplace(SelectableWidget);
    });

    return Ret;
}

bool UScreen::GiveMenuFocus() {
    bool FocusGranted = false;
    UWidgetTree::ForWidgetAndChildren(WidgetTree->RootWidget, [&FocusGranted](UWidget *Widget) {
        auto SelectableWidget = Cast<USelectableWidget>(Widget);
        if (SelectableWidget == nullptr)
            return;

        if (SelectableWidget->IsActive()) {
            FocusGranted = true;
            SelectableWidget->SetKeyboardFocus();
        }
    });

    return FocusGranted;
}

void UScreen::CloseScreen() {
    GetOwningPlayer()->GetLocalPlayer()->GetSubsystem<URPGMenusSubsystem>()->RemoveScreenFromStack();
    OnScreenClosed.Broadcast();
}

FOnScreenClosed & UScreen::GetOnScreenClosed() {
    return OnScreenClosed;
}
