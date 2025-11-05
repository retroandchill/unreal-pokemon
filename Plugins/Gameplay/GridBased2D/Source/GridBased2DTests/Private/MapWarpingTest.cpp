// "Unreal Pokémon" created by Retro & Chill.

#include "MapWarpingTest.h"
#include "Map/MapSubsystem.h"

void AMapWarpingTest::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    if (EndPlayReason == EEndPlayReason::Type::LevelTransition) {
        auto Subsystem = GetGameInstance()->GetSubsystem<UMapSubsystem>();
        AssertTrue(Subsystem->GetWarpDestination().IsSet(), TEXT("Warp destination is set"));
        FinishTest(EFunctionalTestResult::Succeeded, TEXT("Test succeeded!"));
    }
    Super::EndPlay(EndPlayReason);
}