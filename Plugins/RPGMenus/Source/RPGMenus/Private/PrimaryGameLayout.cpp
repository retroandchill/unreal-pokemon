// "Unreal Pokémon" created by Retro & Chill.

#include "PrimaryGameLayout.h"
#include "GameUIPolicy.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RPGLocalPlayer.h"
#include "RetroLib/Casting/DynamicCast.h"
#include "RetroLib/Optionals/OptionalOperations.h"
#include "RetroLib/Optionals/PtrOrNull.h"
#include "RetroLib/Optionals/Transform.h"
#include "RPGMenus.h"
#include "RPGUIManagerSubsystem.h"

UPrimaryGameLayout *UPrimaryGameLayout::Get(const UObject *WorldContextObject)
{
    auto GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
    auto PlayerController = GameInstance->GetPrimaryPlayerController(false);
    return Get(PlayerController);
}

UPrimaryGameLayout *UPrimaryGameLayout::Get(const APlayerController *PlayerController)
{
    return PlayerController ? Get(Cast<URPGLocalPlayer>(PlayerController->Player)) : nullptr;
}

UPrimaryGameLayout *UPrimaryGameLayout::Get(ULocalPlayer *LocalPlayer)
{
    // clang-format off
    auto CommonLocalPlayer = Retro::Optionals::OfNullable(LocalPlayer) |
                             Retro::Optionals::Transform(Retro::DynamicCastChecked<URPGLocalPlayer>);
    return CommonLocalPlayer |
           Retro::Optionals::Transform(&URPGLocalPlayer::GetGameInstance) |
           Retro::Optionals::Transform([](const UGameInstance &GameInstance) {
               return GameInstance.GetSubsystem<URPGUIManagerSubsystem>();
           }) |
           Retro::Optionals::Transform([](const URPGUIManagerSubsystem &Subsystem) {
               return Subsystem.GetCurrentUIPolicy();
           }) |
           Retro::Optionals::Transform(Retro::BindBack<&UGameUIPolicy::GetRootLayout>(CommonLocalPlayer.GetPtrOrNull()))
           |
           Retro::Optionals::PtrOrNull;
    // clang-format on
}

void UPrimaryGameLayout::SetIsDormant(bool Dormant)
{
    if (bIsDormant == Dormant)
    {
        return;
    }

    auto LP = GetOwningLocalPlayer();
    const int32 PlayerId = LP ? LP->GetControllerId() : -1;
    auto OldDormancyStr = bIsDormant ? TEXT("Dormant") : TEXT("Not-Dormant");
    auto NewDormancyStr = Dormant ? TEXT("Dormant") : TEXT("Not-Dormant");
    auto PrimaryPlayerStr = LP && LP->IsPrimaryPlayer() ? TEXT("[Primary]") : TEXT("[Non-Primary]");
    UE_LOG(LogRPGMenus, Display, TEXT("%s PrimaryGameLayout Dormancy changed for [%d] from [%s] to [%s]"),
           PrimaryPlayerStr, PlayerId, OldDormancyStr, NewDormancyStr);

    bIsDormant = Dormant;
    OnIsDormantChanged();
}

void UPrimaryGameLayout::FindAndRemoveWidgetFromLayer(UCommonActivatableWidget *ActivatableWidget)
{
    // We're not sure what layer the widget is on so go searching.
    for (const auto &[Key, Value] : Layers)
    {
        Value->RemoveWidget(*ActivatableWidget);
    }
}

UCommonActivatableWidgetContainerBase *UPrimaryGameLayout::GetLayerWidget(FGameplayTag LayerName)
{
    return Layers.FindRef(LayerName);
}

void UPrimaryGameLayout::RegisterLayer(FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase *LayerWidget)
{
    if (IsDesignTime())
    {
        return;
    }

    LayerWidget->OnTransitioningChanged.AddUObject(this, &UPrimaryGameLayout::OnWidgetStackTransitioning);
    // TODO: Consider allowing a transition duration, we currently set it to 0, because if it's not 0, the
    //       transition effect will cause focus to not transition properly to the new widgets when using
    //       gamepad always.
    LayerWidget->SetTransitionDuration(0.0);

    Layers.Add(LayerTag, LayerWidget);
}

void UPrimaryGameLayout::OnIsDormantChanged()
{
    // Currently no processing
}

void UPrimaryGameLayout::OnWidgetStackTransitioning(UCommonActivatableWidgetContainerBase *, bool bIsTransitioning)
{
    if (bIsTransitioning)
    {
        const FName SuspendToken =
            UCommonUIExtensions::SuspendInputForPlayer(GetOwningLocalPlayer(), TEXT("GlobalStackTransion"));
        SuspendInputTokens.Add(SuspendToken);
    }
    else
    {
        if (ensure(SuspendInputTokens.Num() > 0))
        {
            const FName SuspendToken = SuspendInputTokens.Pop();
            UCommonUIExtensions::ResumeInputForPlayer(GetOwningLocalPlayer(), SuspendToken);
        }
    }
}