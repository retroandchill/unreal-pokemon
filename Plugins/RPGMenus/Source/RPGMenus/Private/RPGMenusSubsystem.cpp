// "Unreal Pokémon" created by Retro & Chill.

#include "RPGMenusSubsystem.h"
#include "Kismet/GameplayStatics.h"

UScreen *URPGMenusSubsystem::AddScreenToStackHelper(const UObject *WorldContextObject,
                                                    TSubclassOf<UScreen> ScreenType) {
    if (ScreenType == nullptr)
        return nullptr;

    if (auto Subsystem = GetSubsystem(WorldContextObject); Subsystem != nullptr) {
        return Subsystem->AddScreenToStack(ScreenType);
    }

    return nullptr;
}

UScreen *URPGMenusSubsystem::GetTopScreenOfStack() const {
    return GetTopOfStack<UScreen>();
}

UScreen *URPGMenusSubsystem::RemoveScreenFromStack() {
    if (ScreenStack.IsEmpty())
        return nullptr;

    UScreen *PoppedWidget = ScreenStack.Pop();
    PoppedWidget->RemoveFromParent();
    if (ScreenStack.IsEmpty()) {
        GetPlayerController()->SetInputMode(FInputModeGameOnly());
        return nullptr;
    }

    UScreen *NewTop = ScreenStack.Top();
    NewTop->GiveMenuFocus();
    return NewTop;
}

URPGMenusSubsystem *URPGMenusSubsystem::GetSubsystem(const UObject *WorldContextObject) {
    if (auto ImpliedOwningPlayer = Cast<APlayerController>(WorldContextObject); ImpliedOwningPlayer != nullptr) {
        return ImpliedOwningPlayer->GetLocalPlayer()->GetSubsystem<URPGMenusSubsystem>();
    }

    if (auto OwningWidget = Cast<UUserWidget>(WorldContextObject); OwningWidget != nullptr) {
        auto PlayerController = OwningWidget->GetOwningPlayer();
        return PlayerController->GetLocalPlayer()->GetSubsystem<URPGMenusSubsystem>();
    }

    if (auto World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull)) {
        auto PlayerController = World->GetFirstPlayerController();
        if (PlayerController != nullptr) {
            return PlayerController->GetLocalPlayer()->GetSubsystem<URPGMenusSubsystem>();
        }
    }

    return nullptr;
}

UScreen *URPGMenusSubsystem::RemoveScreenFromStack(const UObject *WorldContextObject) {
    if (auto Subsystem = GetSubsystem(WorldContextObject); Subsystem != nullptr) {
        return Subsystem->RemoveScreenFromStack();
    }

    return nullptr;
}

APlayerController *URPGMenusSubsystem::GetPlayerController() const {
    return GetLocalPlayer()->GetPlayerController(nullptr);
}