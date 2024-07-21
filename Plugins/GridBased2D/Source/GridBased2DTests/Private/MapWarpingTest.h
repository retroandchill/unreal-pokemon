// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "MapWarpingTest.generated.h"

UCLASS(Abstract)
class GRIDBASED2DTESTS_API AMapWarpingTest : public AFunctionalTest {
    GENERATED_BODY()

public:
    void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};