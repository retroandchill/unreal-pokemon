// "Unreal Pokémon" created by Retro & Chill.


#include "GameUIPolicy.h"
#include "PrimaryGameLayout.h"
#include "RPGMenus.h"
#include "RPGUIManagerSubsystem.h"
#include "Player/RPGLocalPlayer.h"
#include "RetroLib/Optionals/OptionalOperations.h"
#include "RetroLib/Optionals/PtrOrNull.h"
#include "RetroLib/Optionals/Transform.h"
#include "RetroLib/Utils/WorldUtils.h"

UGameUIPolicy *UGameUIPolicy::Get(const UObject *WorldContextObject) {
    // clang-format off
    return Retro::Optionals::OfNullable(
               GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull)) |
           Retro::Optionals::Transform(Retro::GetGameInstance) |
           Retro::Optionals::Transform(Retro::GetSubsystem<URPGUIManagerSubsystem>) |
           Retro::Optionals::Transform(RPG::Menus::GetCurrentUIPolicy) |
           Retro::Optionals::PtrOrNull;
    // clang-format on
}

UWorld *UGameUIPolicy::GetWorld() const {
    return GetOwningUIManager()->GetGameInstance()->GetWorld();
}

URPGUIManagerSubsystem *UGameUIPolicy::GetOwningUIManager() const {
    return CastChecked<URPGUIManagerSubsystem>(GetOuter());
}

UPrimaryGameLayout *UGameUIPolicy::GetRootLayout(const URPGLocalPlayer *LocalPlayer) const {
    const auto LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer);
    return LayoutInfo ? LayoutInfo->RootLayout : nullptr;
}

void UGameUIPolicy::RequestPrimaryControl(UPrimaryGameLayout *Layout) {
    if (LocalMultiplayerInteractionMode != ELocalMultiplayerInteractionMode::SingleToggle || !Layout->IsDormant()) {
        return;
    }

    for (const FRootViewportLayoutInfo &LayoutInfo : RootViewportLayouts) {
        if (UPrimaryGameLayout *RootLayout = LayoutInfo.RootLayout; RootLayout && !RootLayout->IsDormant()) {
            RootLayout->SetIsDormant(true);
            break;
        }
    }
    Layout->SetIsDormant(false);
}

void UGameUIPolicy::AddLayoutToViewport(URPGLocalPlayer *LocalPlayer, UPrimaryGameLayout *Layout) {
    UE_LOG(LogRPGMenus, Log, TEXT("[%s] is adding player [%s]'s root layout [%s] to the viewport"), *GetName(),
           *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));

    Layout->SetPlayerContext(FLocalPlayerContext(LocalPlayer));
    Layout->AddToPlayerScreen(1000);

    OnRootLayoutAddedToViewport(LocalPlayer, Layout);
}

void UGameUIPolicy::RemoveLayoutFromViewport(URPGLocalPlayer *LocalPlayer, UPrimaryGameLayout *Layout) {
    TWeakPtr<SWidget> LayoutSlateWidget = Layout->GetCachedWidget();
    if (!LayoutSlateWidget.IsValid()) {
        return;
    }

    UE_LOG(LogRPGMenus, Log, TEXT("[%s] is removing player [%s]'s root layout [%s] from the viewport"), *GetName(),
           *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));

    Layout->RemoveFromParent();
    if (LayoutSlateWidget.IsValid()) {
        UE_LOG(LogRPGMenus, Log,
               TEXT(
                   "Player [%s]'s root layout [%s] has been removed from the viewport, but other references to its underlying Slate widget still exist. Noting in case we leak it."
               ), *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));
    }

    OnRootLayoutRemovedFromViewport(LocalPlayer, Layout);
}

void UGameUIPolicy::OnRootLayoutAddedToViewport(URPGLocalPlayer *LocalPlayer, UPrimaryGameLayout *) {
#if WITH_EDITOR
    if (GIsEditor && LocalPlayer->IsPrimaryPlayer()) {
        // So our controller will work in PIE without needing to click in the viewport
        FSlateApplication::Get().SetUserFocusToGameViewport(0);
    }
#endif
}

void UGameUIPolicy::OnRootLayoutRemovedFromViewport(URPGLocalPlayer *, UPrimaryGameLayout *) {
    // No processing needed
}

void UGameUIPolicy::OnRootLayoutReleased(URPGLocalPlayer *, UPrimaryGameLayout *) {
    // No processing needed
}

void UGameUIPolicy::CreateLayoutWidget(URPGLocalPlayer *LocalPlayer) {
    auto PlayerController = LocalPlayer->GetPlayerController(GetWorld());
    if (PlayerController == nullptr) {
        return;
    }

    if (auto LayoutWidgetClass = GetLayoutWidgetClass(LocalPlayer); ensure(
        LayoutWidgetClass != nullptr && !LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract))) {
        UPrimaryGameLayout *NewLayoutObject = CreateWidget<UPrimaryGameLayout>(PlayerController, LayoutWidgetClass);
        RootViewportLayouts.Emplace(LocalPlayer, NewLayoutObject, true);

        AddLayoutToViewport(LocalPlayer, NewLayoutObject);
    }
}

TSubclassOf<UPrimaryGameLayout> UGameUIPolicy::GetLayoutWidgetClass(URPGLocalPlayer *) {
    return LayoutClass.LoadSynchronous();
}

void UGameUIPolicy::NotifyPlayerAdded(URPGLocalPlayer *LocalPlayer) {
    LocalPlayer->BindToOnPlayerControllerSet(this, [this](URPGLocalPlayer *Self, APlayerController *PlayerController) {
        NotifyPlayerRemoved(Self);

        if (FRootViewportLayoutInfo *LayoutInfo = RootViewportLayouts.FindByKey(Self)) {
            AddLayoutToViewport(Self, LayoutInfo->RootLayout);
            LayoutInfo->bAddedToViewport = true;
        } else {
            CreateLayoutWidget(Self);
        }
    });

    if (auto *LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer)) {
        AddLayoutToViewport(LocalPlayer, LayoutInfo->RootLayout);
        LayoutInfo->bAddedToViewport = true;
    } else {
        CreateLayoutWidget(LocalPlayer);
    }
}

void UGameUIPolicy::NotifyPlayerRemoved(URPGLocalPlayer *LocalPlayer) {
    auto *LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer);
    if (LayoutInfo == nullptr) {
        return;
    }
    RemoveLayoutFromViewport(LocalPlayer, LayoutInfo->RootLayout);
    LayoutInfo->bAddedToViewport = false;

    if (LocalMultiplayerInteractionMode != ELocalMultiplayerInteractionMode::SingleToggle || LocalPlayer->
        IsPrimaryPlayer()) {
        return;
    }

    UPrimaryGameLayout *RootLayout = LayoutInfo->RootLayout;
    if (RootLayout == nullptr || RootLayout->IsDormant()) {
        return;
    }

    // We're removing a secondary player's root while it's in control - transfer control back to the primary player's root
    RootLayout->SetIsDormant(true);
    for (const FRootViewportLayoutInfo &RootLayoutInfo : RootViewportLayouts) {
        if (!RootLayoutInfo.LocalPlayer->IsPrimaryPlayer()) {
            continue;
        }

        if (UPrimaryGameLayout *PrimaryRootLayout = RootLayoutInfo.RootLayout; PrimaryRootLayout != nullptr) {
            PrimaryRootLayout->SetIsDormant(false);
        }
    }
}

void UGameUIPolicy::NotifyPlayerDestroyed(URPGLocalPlayer *LocalPlayer) {
    NotifyPlayerRemoved(LocalPlayer);
    LocalPlayer->RemoveAllFromOnPlayerControllerSet(this);
    if (const int32 LayoutInfoIdx = RootViewportLayouts.IndexOfByKey(LocalPlayer); LayoutInfoIdx != INDEX_NONE) {
        UPrimaryGameLayout *Layout = RootViewportLayouts[LayoutInfoIdx].RootLayout;
        RootViewportLayouts.RemoveAt(LayoutInfoIdx);

        RemoveLayoutFromViewport(LocalPlayer, Layout);

        OnRootLayoutReleased(LocalPlayer, Layout);
    }
}