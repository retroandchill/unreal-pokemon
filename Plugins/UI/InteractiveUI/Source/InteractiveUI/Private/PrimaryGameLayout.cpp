// "Unreal Pokémon" created by Retro & Chill.

#include "PrimaryGameLayout.h"
#include "GameUIManagerSubsystem.h"
#include "GameUIPolicy.h"
#include "OptionalPtr.h"
#include "InteractiveUI.h"


UPrimaryGameLayout* UPrimaryGameLayout::GetInstance(const UObject* WorldContextObject)
{
    const auto GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
    const auto PlayerController = GameInstance->GetPrimaryPlayerController(false);
    return GetInstance(PlayerController);
}

UPrimaryGameLayout* UPrimaryGameLayout::GetInstance(const APlayerController* PlayerController)
{
    return PlayerController != nullptr ? GetInstance(Cast<ULocalPlayer>(PlayerController->Player)) : nullptr;
}

UPrimaryGameLayout* UPrimaryGameLayout::GetInstance(ULocalPlayer* LocalPlayer)
{
    return TOptionalPtr(LocalPlayer)
        .Map(&ULocalPlayer::GetGameInstance)
        .Map([](const UGameInstance* GameInstance) { return GameInstance->GetSubsystem<UGameUIManagerSubsystem>(); })
        .Map([](const UGameUIManagerSubsystem* Subsystem) { return Subsystem->GetCurrentUIPolicy(); })
        .Map(&UGameUIPolicy::GetRootLayout, LocalPlayer)
        .Get();
}

void UPrimaryGameLayout::SetIsDormant(const bool bNewIsDormant)
{
    if (bIsDormant != bNewIsDormant)
    {
        constexpr auto DormantString = TEXT("Dormant");
        constexpr auto NotDormantString = TEXT("Not-Dormant");
        constexpr auto PrimaryString = TEXT("[Primary]");
        constexpr auto NonPrimaryString = TEXT("[Non-Primary]");
        
        const auto LocalPlayer = GetOwningLocalPlayer();
        const int32 PlayerId = LocalPlayer != nullptr ? LocalPlayer->GetControllerId() : INDEX_NONE;
        const auto OldDormancyStr = bIsDormant ? DormantString : NotDormantString;
		const auto NewDormancyStr = bNewIsDormant ? DormantString : NotDormantString;
		const auto PrimaryPlayerStr = LocalPlayer != nullptr && LocalPlayer->IsPrimaryPlayer() ? PrimaryString : NonPrimaryString;
        UE_LOG(LogInteractiveUI, Display, TEXT("%s PrimaryGameLayout Dormancy changed for [%d] from [%s] to [%s]"), PrimaryPlayerStr, PlayerId, OldDormancyStr, NewDormancyStr);

        bIsDormant = bNewIsDormant;
        OnIsDormantChanged();
    }
}

void UPrimaryGameLayout::FindAndRemoveWidgetFromLayer(UCommonActivatableWidget* WidgetToRemove)
{
    for (const auto& [Key, Value] : Layers)
    {
        Value->RemoveWidget(*WidgetToRemove);
    }
}

UCommonActivatableWidgetContainerBase* UPrimaryGameLayout::GetLayerWidget(const FGameplayTag LayerName) const
{
    return Layers.FindRef(LayerName);
}

void UPrimaryGameLayout::RegisterLayer(const FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget)
{
    if (IsDesignTime()) return;

    LayerWidget->OnTransitioningChanged.AddUObject(this, &UPrimaryGameLayout::OnWidgetStackTransitioning);
    LayerWidget->SetTransitionDuration(0.0f);

    Layers.Add(LayerTag, LayerWidget);
}

void UPrimaryGameLayout::OnIsDormantChanged_Implementation()
{
    // No implementation here
}

void UPrimaryGameLayout::OnWidgetStackTransitioning(UCommonActivatableWidgetContainerBase*,
                                                    const bool bIsTransitioning)
{
    if (bIsTransitioning)
    {
        const FName SuspendToken = UCommonUIExtensions::SuspendInputForPlayer(GetOwningLocalPlayer(), TEXT("GlobalStackTransition"));
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
}\
