// "Unreal Pokémon" created by Retro & Chill.


#include "TestShutdownSubsystem.h"

#include "AutomationBlueprintFunctionLibrary.h"

UTestShutdownSubsystem* UTestShutdownSubsystem::Instance = nullptr;

void UTestShutdownSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);
    Instance = this;
}

void UTestShutdownSubsystem::Deinitialize() {
    Super::Deinitialize();
    Instance = this;
}

UTestShutdownSubsystem & UTestShutdownSubsystem::GetInstance() {
    check(Instance != nullptr)
    return *Instance;
}

TStatId UTestShutdownSubsystem::GetStatId() const {
    RETURN_QUICK_DECLARE_CYCLE_STAT(UTestShutdownSubsystem, STATGROUP_Tickables)
}

bool UTestShutdownSubsystem::IsTickable() const {
    return true;
}

void UTestShutdownSubsystem::Tick(float DeltaTime) {
    if (!UAutomationBlueprintFunctionLibrary::AreAutomatedTestsRunning()) {
        if (bAutomationStarted && bExitRequested) {
            FGenericPlatformMisc::RequestExit(false);
        }
    } else if (!bAutomationStarted) {
        bAutomationStarted = true;
    }
}

void UTestShutdownSubsystem::RequestExitAfterTestsComplete() {
    bExitRequested = true;
}