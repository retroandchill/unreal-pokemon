// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "DestructionAssertTest.generated.h"

/**
 * Specialized test class that is set up to pass when EndPlay performs a final assert that can circumvent the in-built
 * failure upon the end of play.
 */
UCLASS(Abstract)
class AUTOMATIONTESTHELPERS_API ADestructionAssertTest : public AFunctionalTest {
    GENERATED_BODY()

public:
    void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
    /**
     * Perform the final assert(s) before play ends.
     * @param EndPlayReason The reason play ended
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Assertions, meta = (ForceAsFunction))
    void FinalAssert(EEndPlayReason::Type EndPlayReason);

};