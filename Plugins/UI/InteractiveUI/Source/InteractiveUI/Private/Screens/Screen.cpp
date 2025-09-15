// "Unreal Pokémon" created by Retro & Chill.
#include "Screens/Screen.h"
#include "Blueprint/WidgetTree.h"
#include "Components/SelectableWidget.h"
#include "PrimaryGameLayout.h"
#include "GameUIManagerSubsystem.h"

class UEnhancedInputLocalPlayerSubsystem;
const UE_DEFINE_GAMEPLAY_TAG(RPG::Menus::PrimaryMenuLayerTag, "UI.Layer.GameMenu");
const UE_DEFINE_GAMEPLAY_TAG(RPG::Menus::OverlayMenuLayerTag, "UI.Layer.Overlay");

void UScreen::RefreshSelf_Implementation()
{
    // No base implementation
}

void UScreen::NativeConstruct()
{
    Super::NativeConstruct();

    SelectableWidgets.Empty();
    WidgetTree->ForEachWidget([this](UWidget *Widget) {
        auto SelectableWidget = Cast<USelectableWidget>(Widget);
        if (SelectableWidget == nullptr)
            return;

        SelectableWidgets.Emplace(SelectableWidget);
    });
}

UWidget *UScreen::NativeGetDesiredFocusTarget() const
{
    auto Widget = SelectableWidgets.FindByPredicate(&UCommonActivatableWidget::IsActivated);
    return Widget != nullptr ? *Widget : nullptr;
}

TOptional<FUIInputConfig> UScreen::GetDesiredInputConfig() const
{
    using enum ERPGWidgetInputMode;

    switch (InputConfig)
    {
    case GameAndMenu:
        return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
    case Game:
        return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
    case Menu:
        return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
    default:
        return TOptional<FUIInputConfig>();
    }
}

void UScreen::CloseScreen()
{
    DeactivateWidget();
    auto Layout = UPrimaryGameLayout::Get(GetOwningPlayer());
    Layout->FindAndRemoveWidgetFromLayer(this);
    OnScreenClosed.Broadcast();
    RemoveFromParent();
}

FOnScreenClosed &UScreen::GetOnScreenClosed()
{
    return OnScreenClosed;
}

void UScreen::NativeOnActivated()
{
    Super::NativeOnActivated();
    if (GetVisibility() == ESlateVisibility::HitTestInvisible)
    {
        SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
}

void UScreen::NativeOnDeactivated()
{
    Super::NativeOnDeactivated();
    if (IsVisible())
    {
        SetVisibility(ESlateVisibility::HitTestInvisible);
    }
}
