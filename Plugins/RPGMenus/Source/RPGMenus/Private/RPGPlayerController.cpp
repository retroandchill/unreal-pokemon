// "Unreal Pokémon" created by Retro & Chill.
#include "RPGPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Screens/Screen.h"

ARPGPlayerController::ARPGPlayerController(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer) {
}

void ARPGPlayerController::BeginPlay() {
    Super::BeginPlay();
    SetInputMode(FInputModeGameOnly());
}

UScreen *ARPGPlayerController::AddScreenToStackHelper(UObject *WorldContextObject, TSubclassOf<UScreen> ScreenType) {
    if (ScreenType == nullptr)
        return nullptr;

    if (auto ImpliedOwningPlayer = Cast<ARPGPlayerController>(WorldContextObject); ImpliedOwningPlayer != nullptr) {
        return ImpliedOwningPlayer->AddScreenToStack(ScreenType);
    }

    if (auto OwningWidget = Cast<UUserWidget>(WorldContextObject); OwningWidget != nullptr) {
        if (auto RPGPlayerController = Cast<ARPGPlayerController>(OwningWidget->GetOwningPlayer());
            RPGPlayerController != nullptr) {
            return RPGPlayerController->AddScreenToStack(ScreenType);
        }
    }

    if (auto World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull)) {
        if (auto RPGPlayerController = Cast<ARPGPlayerController>(World->GetFirstPlayerController());
            RPGPlayerController != nullptr) {
            return RPGPlayerController->AddScreenToStack(ScreenType);
        }
    }

    return nullptr;
}

UScreen *ARPGPlayerController::GetTopScreenOfStack() const { return GetTopOfStack<UScreen>(); }

UScreen *ARPGPlayerController::RemoveScreenFromStack() {
    if (ScreenStack.IsEmpty())
        return nullptr;

    UScreen *PoppedWidget = ScreenStack.Pop();
    PoppedWidget->RemoveFromParent();
    if (ScreenStack.IsEmpty()) {
        SetInputMode(FInputModeGameOnly());
        return nullptr;
    }

    UScreen *NewTop = ScreenStack.Top();
    NewTop->GiveMenuFocus();
    return NewTop;
}

UScreen *ARPGPlayerController::RemoveScreenFromStack(UObject *WorldContextObject) {
    if (auto ImpliedOwningPlayer = Cast<ARPGPlayerController>(WorldContextObject); ImpliedOwningPlayer != nullptr) {
        return ImpliedOwningPlayer->RemoveScreenFromStack();
    }

    if (auto OwningWidget = Cast<UUserWidget>(WorldContextObject); OwningWidget != nullptr) {
        if (auto RPGPlayerController = Cast<ARPGPlayerController>(OwningWidget->GetOwningPlayer());
            RPGPlayerController != nullptr) {
            return RPGPlayerController->RemoveScreenFromStack();
        }
    }

    if (auto World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull)) {
        if (auto RPGPlayerController = Cast<ARPGPlayerController>(World->GetFirstPlayerController());
            RPGPlayerController != nullptr) {
            return RPGPlayerController->RemoveScreenFromStack();
        }
    }

    return nullptr;
}
