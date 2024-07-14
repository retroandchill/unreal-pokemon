// "Unreal Pokémon" created by Retro & Chill.
#include "Screens/Screen.h"
#include "Blueprint/WidgetTree.h"
#include "RPGMenusSubsystem.h"
#include "Windows/SelectableWidget.h"

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

void UScreen::RefreshSelf_Implementation() {
    // No base implementation
}

void UScreen::CloseScreen() {
    GetOwningPlayer()->GetLocalPlayer()->GetSubsystem<URPGMenusSubsystem>()->RemoveScreenFromStack();
    OnScreenClosed.Broadcast();
}

FOnScreenClosed &UScreen::GetOnScreenClosed() {
    return OnScreenClosed;
}

void UScreen::NativeOnActivated() {
    Super::NativeOnActivated();
    if (GetVisibility() == ESlateVisibility::HitTestInvisible) {
        SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
}

void UScreen::NativeOnDeactivated() {
    Super::NativeOnDeactivated();
    if (IsVisible()) {
        SetVisibility(ESlateVisibility::HitTestInvisible);
    }
}
