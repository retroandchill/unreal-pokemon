// "Unreal Pokémon" created by Retro & Chill.

#include "TestShutdownSubsystem.h"
#include "IAutomationControllerModule.h"
#include "TestShutdownOutputDevice.h"
#include "Algo/AnyOf.h"

class IAutomationControllerModule;
UTestShutdownSubsystem *UTestShutdownSubsystem::Instance = nullptr;
TUniquePtr<FTestShutdownOutputDevice> UTestShutdownSubsystem::ShutdownOutputDevice =
    MakeUnique<FTestShutdownOutputDevice>();

void UTestShutdownSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);
    Instance = this;
    GLog->AddOutputDevice(ShutdownOutputDevice.Get());
}

void UTestShutdownSubsystem::Deinitialize() {
    Super::Deinitialize();
    Instance = this;
}

UTestShutdownSubsystem &UTestShutdownSubsystem::GetInstance() {
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
    if (ShutdownOutputDevice->ShutdownMessageReceived() && bExitRequested) {
        auto AutomationControllerModule = FModuleManager::GetModulePtr<IAutomationControllerModule>("AutomationController");
        auto AutomationController = AutomationControllerModule->GetAutomationController();
        uint8 ExitCode = Algo::AnyOf(AutomationController->GetFilteredReports(), &IAutomationReport::HasErrors) ? 1 : 0;
        FGenericPlatformMisc::RequestExitWithStatus(false, ExitCode);
    }
}

void UTestShutdownSubsystem::RequestExitAfterTestsComplete() {
    bExitRequested = true;
}