// "Unreal Pokémon" created by Retro & Chill.
#include "Screens/Screen.h"
#include "PrimaryGameLayout.h"
#include "Blueprint/WidgetTree.h"
#include "Windows/SelectableWidget.h"


UE_DEFINE_GAMEPLAY_TAG(RPG::Menus::PrimaryMenuLayerTag, "UI.Layer.GameMenu");
UE_DEFINE_GAMEPLAY_TAG(RPG::Menus::OverlayMenuLayerTag, "UI.Layer.Overlay");

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
    UPrimaryGameLayout::GetPrimaryGameLayout(GetOwningPlayer())->FindAndRemoveWidgetFromLayer(this);
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
