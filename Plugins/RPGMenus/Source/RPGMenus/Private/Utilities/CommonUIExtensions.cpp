// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/CommonUIExtensions.h"
#include "CommonInputSubsystem.h"

int32 UCommonUIExtensions::InputSuspensions = 0;

FName UCommonUIExtensions::SuspendInputForPlayer(APlayerController *PlayerController, FName SuspendReason) {
    return SuspendInputForPlayer(PlayerController != nullptr ? PlayerController->GetLocalPlayer() : nullptr, SuspendReason);
}

FName UCommonUIExtensions::SuspendInputForPlayer(const ULocalPlayer *LocalPlayer, FName SuspendReason) {
    auto CommonInputSubsystem = UCommonInputSubsystem::Get(LocalPlayer);
    if (CommonInputSubsystem == nullptr) {
        return NAME_None;
    }

    InputSuspensions++;
    auto SuspendToken = SuspendReason;
    SuspendToken.SetNumber(InputSuspensions);

    CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::MouseAndKeyboard, SuspendToken, true);
    CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Gamepad, SuspendToken, true);
    CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Touch, SuspendToken, true);

    return SuspendToken;
}

void UCommonUIExtensions::ResumeInputForPlayer(APlayerController *PlayerController, FName SuspendToken) {
    ResumeInputForPlayer(PlayerController != nullptr ? PlayerController->GetLocalPlayer() : nullptr, SuspendToken);
}

void UCommonUIExtensions::ResumeInputForPlayer(const ULocalPlayer *LocalPlayer, FName SuspendToken) {
    if (SuspendToken == NAME_None) {
        return;
    }

    if (auto CommonInputSubsystem = UCommonInputSubsystem::Get(LocalPlayer); CommonInputSubsystem != nullptr) {
        CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::MouseAndKeyboard, SuspendToken, false);
        CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Gamepad, SuspendToken, false);
        CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Touch, SuspendToken, false);
    }
}