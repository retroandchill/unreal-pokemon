// "Unreal Pokémon" created by Retro & Chill.


#include "TestShutdownSubsystem.h"

#include "TestShutdownOutputDevice.h"

UTestShutdownSubsystem* UTestShutdownSubsystem::Instance = nullptr;
TUniquePtr<FTestShutdownOutputDevice> UTestShutdownSubsystem::ShutdownOutputDevice = MakeUnique<FTestShutdownOutputDevice>();

void UTestShutdownSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);
    Instance = this;
    GLog->AddOutputDevice(ShutdownOutputDevice.Get());
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
    if (ShutdownOutputDevice->ShutdownMessageReceived() && bExitRequested) {
        FGenericPlatformMisc::RequestExit(false);
    }
}

void UTestShutdownSubsystem::RequestExitAfterTestsComplete() {
    bExitRequested = true;
}