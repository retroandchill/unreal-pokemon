// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/CommonUIExtensions.h"
#include "CommonInputSubsystem.h"
#include "GameUIManagerSubsystem.h"
#include "GameUIPolicy.h"
#include "OptionalPtr.h"
#include "PrimaryGameLayout.h"
#include "Blueprint/UserWidget.h"

int32 UCommonUIExtensions::InputSuspensions = 0;

ECommonInputType UCommonUIExtensions::GetOwningPlayerInputType(const UUserWidget *WidgetContextObject)
{
    return TOptionalPtr(WidgetContextObject)
        .Map([](const UUserWidget* Widget) { return Widget->GetOwningLocalPlayer(); })
        .Map(&UCommonInputSubsystem::Get)
        .MapToValue(ECommonInputType::Count, &UCommonInputSubsystem::GetCurrentInputType);
}



static bool IsOwningPlayerInputType(const UUserWidget* WidgetContextObject, ECommonInputType InputType)
{
    return TOptionalPtr(WidgetContextObject)
        .Map([](const UUserWidget* Widget) { return Widget->GetOwningLocalPlayer(); })
        .Map(&UCommonInputSubsystem::Get)
        .MapToValue(false, [InputType](const UCommonInputSubsystem* Subsystem) { return Subsystem->GetCurrentInputType() == InputType; });
}

bool UCommonUIExtensions::IsOwningPlayerUsingTouch(const UUserWidget *WidgetContextObject)
{
    return IsOwningPlayerInputType(WidgetContextObject, ECommonInputType::Touch);
}

bool UCommonUIExtensions::IsOwningPlayerUsingGamepad(const UUserWidget *WidgetContextObject)
{
    return IsOwningPlayerInputType(WidgetContextObject, ECommonInputType::Gamepad);
}

UCommonActivatableWidget * UCommonUIExtensions::PushContentToLayer(const ULocalPlayer *LocalPlayer,
    FGameplayTag LayerName, TSubclassOf<UCommonActivatableWidget> WidgetClass)
{
    if (ensure(LocalPlayer != nullptr) || !ensure(WidgetClass != nullptr))
    {
        return nullptr;
    }

    return TOptionalPtr(LocalPlayer->GetGameInstance()->GetSubsystem<UGameUIManagerSubsystem>())
        .Map([](UGameUIManagerSubsystem* Subsystem) { return Subsystem->GetCurrentUIPolicy(); })
        .Map([LocalPlayer](const UGameUIPolicy* Policy) { return Policy->GetRootLayout(LocalPlayer); })
        .Map([LayerName, WidgetClass](UPrimaryGameLayout* Layout) { return Layout->PushWidgetToLayerStack(LayerName, WidgetClass); })
        .Get();
}

void UCommonUIExtensions::PushStreamedContentToLayer(const ULocalPlayer *LocalPlayer, FGameplayTag LayerName,
    TSoftClassPtr<UCommonActivatableWidget> WidgetClass)
{
    if (ensure(LocalPlayer != nullptr) || !ensure(WidgetClass != nullptr))
    {
        return;
    }

    TOptionalPtr(LocalPlayer->GetGameInstance()->GetSubsystem<UGameUIManagerSubsystem>())
        .Map([](UGameUIManagerSubsystem* Subsystem) { return Subsystem->GetCurrentUIPolicy(); })
        .Map([LocalPlayer](const UGameUIPolicy* Policy) { return Policy->GetRootLayout(LocalPlayer); })
        .IfPresent ([LayerName, &WidgetClass](UPrimaryGameLayout* Layout)
        {
            return Layout->PushWidgetToLayerStackAsync(LayerName, true, MoveTemp(WidgetClass));
        });
}

void UCommonUIExtensions::PopContentFromLayer(UCommonActivatableWidget *ActivatableWidget)
{
    if (ActivatableWidget == nullptr)
    {
        // Ignore request to pop an already deleted widget
        return;
    }

    const auto LocalPlayer = TOptionalPtr(ActivatableWidget)
        .Map([](const UCommonActivatableWidget* Widget) { return Widget->GetOwningLocalPlayer(); })
        .Get();
    
    TOptionalPtr(LocalPlayer)
        .Map([](const ULocalPlayer* Player) { return Player->GetGameInstance()->GetSubsystem<UGameUIManagerSubsystem>(); })
        .Map([](const UGameUIManagerSubsystem* Subsystem) { return Subsystem->GetCurrentUIPolicy(); })
        .Map([LocalPlayer](const UGameUIPolicy* Policy) { return Policy->GetRootLayout(LocalPlayer); })
        .IfPresent([ActivatableWidget](UPrimaryGameLayout* Layout)
        {
            Layout->FindAndRemoveWidgetFromLayer(ActivatableWidget);
        });
}

ULocalPlayer * UCommonUIExtensions::GetLocalPlayerFromController(APlayerController *PlayerController)
{
    return PlayerController != nullptr ? PlayerController->GetLocalPlayer() : nullptr;
}

FName UCommonUIExtensions::SuspendInputForPlayer(APlayerController *PlayerController, FName SuspendReason)
{
    return SuspendInputForPlayer(PlayerController != nullptr ? PlayerController->GetLocalPlayer() : nullptr,
                                 SuspendReason);
}

FName UCommonUIExtensions::SuspendInputForPlayer(const ULocalPlayer *LocalPlayer, FName SuspendReason)
{
    auto *CommonInputSubsystem = UCommonInputSubsystem::Get(LocalPlayer);
    if (CommonInputSubsystem == nullptr)
    {
        return NAME_None;
    }

    InputSuspensions++;
    auto SuspendToken = SuspendReason;
    SuspendToken.SetNumber(InputSuspensions);

    using enum ECommonInputType;
    CommonInputSubsystem->SetInputTypeFilter(MouseAndKeyboard, SuspendToken, true);
    CommonInputSubsystem->SetInputTypeFilter(Gamepad, SuspendToken, true);
    CommonInputSubsystem->SetInputTypeFilter(Touch, SuspendToken, true);

    return SuspendToken;
}

void UCommonUIExtensions::ResumeInputForPlayer(APlayerController *PlayerController, const FName SuspendToken)
{
    ResumeInputForPlayer(PlayerController != nullptr ? PlayerController->GetLocalPlayer() : nullptr, SuspendToken);
}

void UCommonUIExtensions::ResumeInputForPlayer(const ULocalPlayer *LocalPlayer, const FName SuspendToken)
{
    if (SuspendToken == NAME_None)
    {
        return;
    }

    if (auto *CommonInputSubsystem = UCommonInputSubsystem::Get(LocalPlayer); CommonInputSubsystem != nullptr)
    {
        using enum ECommonInputType;
        CommonInputSubsystem->SetInputTypeFilter(MouseAndKeyboard, SuspendToken, false);
        CommonInputSubsystem->SetInputTypeFilter(Gamepad, SuspendToken, false);
        CommonInputSubsystem->SetInputTypeFilter(Touch, SuspendToken, false);
    }
}