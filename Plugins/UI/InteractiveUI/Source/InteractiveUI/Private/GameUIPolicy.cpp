// "Unreal Pokémon" created by Retro & Chill.

#include "GameUIPolicy.h"
#include "GameUIManagerSubsystem.h"
#include "InteractiveUI.h"
#include "OptionalPtr.h"
#include "PrimaryGameLayout.h"

UGameUIPolicy *UGameUIPolicy::GetInstance(const UObject *WorldContextObject)
{
    return TOptionalPtr(GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
        .Map([](const UWorld *World) { return World->GetGameInstance(); })
        .Map([](const UGameInstance *GameInstance) { return GameInstance->GetSubsystem<UGameUIManagerSubsystem>(); })
        .Map([](UGameUIManagerSubsystem *Subsystem) { return Subsystem->GetCurrentUIPolicy(); })
        .Get();
}

UWorld *UGameUIPolicy::GetWorld() const
{
    return GetOwner()->GetGameInstance()->GetWorld();
}

UGameUIManagerSubsystem *UGameUIPolicy::GetOwner() const
{
    return CastChecked<UGameUIManagerSubsystem>(GetOuter());
}

UPrimaryGameLayout *UGameUIPolicy::GetRootLayout(const ULocalPlayer *Player) const
{
    const auto LayoutInfo = RootViewportLayouts.FindByKey(Player);
    return LayoutInfo != nullptr ? LayoutInfo->RootLayout : nullptr;
}

void UGameUIPolicy::RequestPrimaryControl(UPrimaryGameLayout *Layout)
{
    if (LocalMultiplayerInteractionMode != ELocalMultiplayerInteractionMode::SingleToggle || !Layout->IsDormant())
    {
        return;
    }

    for (const auto &LayoutInfo : RootViewportLayouts)
    {
        if (UPrimaryGameLayout *RootLayout = LayoutInfo.RootLayout; RootLayout && !RootLayout->IsDormant())
        {
            RootLayout->SetIsDormant(true);
            break;
        }
    }
    Layout->SetIsDormant(false);
}

void UGameUIPolicy::AddLayoutToViewport(ULocalPlayer *LocalPlayer, UPrimaryGameLayout *Layout)
{
    UE_LOG(LogInteractiveUI, Log, TEXT("[%s] is adding player [%s]'s root layout [%s] to the viewport"), *GetName(),
           *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));

    Layout->SetPlayerContext(FLocalPlayerContext(LocalPlayer));
    Layout->AddToPlayerScreen(1000);

    OnRootLayoutAddedToViewport(LocalPlayer, Layout);
}

void UGameUIPolicy::RemoveLayoutFromViewport(ULocalPlayer *LocalPlayer, UPrimaryGameLayout *Layout)
{
    const TWeakPtr<SWidget> LayoutSlateWidget = Layout->GetCachedWidget();
    if (!LayoutSlateWidget.IsValid())
    {
        return;
    }

    UE_LOG(LogInteractiveUI, Log, TEXT("[%s] is removing player [%s]'s root layout [%s] from the viewport"), *GetName(),
           *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));

    Layout->RemoveFromParent();
    if (LayoutSlateWidget.IsValid())
    {
        UE_LOG(LogInteractiveUI, Log,
               TEXT("Player [%s]'s root layout [%s] has been removed from the viewport, but other references to its "
                    "underlying Slate widget still exist. Noting in case we leak it."),
               *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));
    }

    OnRootLayoutRemovedFromViewport(LocalPlayer, Layout);
}

void UGameUIPolicy::OnRootLayoutAddedToViewport_Implementation(ULocalPlayer *LocalPlayer, UPrimaryGameLayout *Layout)
{
#if WITH_EDITOR
    if (GIsEditor && LocalPlayer->IsPrimaryPlayer())
    {
        // So our controller will work in PIE without needing to click in the viewport
        FSlateApplication::Get().SetUserFocusToGameViewport(0);
    }
#endif
}

void UGameUIPolicy::OnRootLayoutRemovedFromViewport_Implementation(ULocalPlayer *LocalPlayer,
                                                                   UPrimaryGameLayout *Layout)
{
    // No implementation in this class
}

void UGameUIPolicy::OnRootLayoutReleased_Implementation(ULocalPlayer *LocalPlayer, UPrimaryGameLayout *Layout)
{
}

void UGameUIPolicy::CreateLayoutWidget(ULocalPlayer *LocalPlayer)
{
    const auto PlayerController = LocalPlayer->GetPlayerController(GetWorld());
    if (PlayerController == nullptr)
        return;

    const auto LayoutWidgetClass = GetLayoutWidgetClass();
    if (!ensure(LayoutWidgetClass && !LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract)))
        return;

    auto NewLayoutObject = CreateWidget<UPrimaryGameLayout>(PlayerController, LayoutWidgetClass);
    RootViewportLayouts.Emplace(LocalPlayer, NewLayoutObject, true);

    AddLayoutToViewport(LocalPlayer, NewLayoutObject);
}

TSubclassOf<UPrimaryGameLayout> UGameUIPolicy::GetLayoutWidgetClass() const
{
    return LayoutClass.LoadSynchronous();
}

void UGameUIPolicy::NotifyPlayerAdded(ULocalPlayer *LocalPlayer)
{
    LocalPlayer->OnPlayerControllerChanged().AddWeakLambda(this, [this, LocalPlayer](APlayerController *) {
        NotifyPlayerRemoved(LocalPlayer);

        if (const auto LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer); LayoutInfo != nullptr)
        {
            AddLayoutToViewport(LocalPlayer, LayoutInfo->RootLayout);
            LayoutInfo->bAddedToViewport = true;
        }
        else
        {
            CreateLayoutWidget(LocalPlayer);
        }
    });

    if (auto *LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer); LayoutInfo != nullptr)
    {
        AddLayoutToViewport(LocalPlayer, LayoutInfo->RootLayout);
        LayoutInfo->bAddedToViewport = true;
    }
    {
        CreateLayoutWidget(LocalPlayer);
    }
}

void UGameUIPolicy::NotifyPlayerRemoved(ULocalPlayer *LocalPlayer)
{
    auto LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer);
    if (LayoutInfo == nullptr)
        return;

    RemoveLayoutFromViewport(LocalPlayer, LayoutInfo->RootLayout);
    LayoutInfo->bAddedToViewport = false;

    if (LocalMultiplayerInteractionMode != ELocalMultiplayerInteractionMode::SingleToggle ||
        LocalPlayer->IsPrimaryPlayer())
        return;

    UPrimaryGameLayout *RootLayout = LayoutInfo->RootLayout;
    if (RootLayout == nullptr || RootLayout->IsDormant())
        return;

    RootLayout->SetIsDormant(true);
    for (auto &RootLayoutInfo : RootViewportLayouts)
    {
        if (!RootLayoutInfo.LocalPlayer->IsPrimaryPlayer())
            continue;

        UPrimaryGameLayout *PrimaryRootLayout = RootLayoutInfo.RootLayout;
        if (PrimaryRootLayout == nullptr)
            continue;

        PrimaryRootLayout->SetIsDormant(false);
    }
}

void UGameUIPolicy::NotifyPlayerDestroyed(ULocalPlayer *LocalPlayer)
{
    NotifyPlayerRemoved(LocalPlayer);
    LocalPlayer->OnPlayerControllerChanged().RemoveAll(this);
    const int32 LayoutInfoIdx = RootViewportLayouts.IndexOfByKey(LocalPlayer);
    if (LayoutInfoIdx == INDEX_NONE)
        return;

    UPrimaryGameLayout *Layout = RootViewportLayouts[LayoutInfoIdx].RootLayout;
    RootViewportLayouts.RemoveAt(LayoutInfoIdx);

    RemoveLayoutFromViewport(LocalPlayer, Layout);

    OnRootLayoutReleased(LocalPlayer, Layout);
}