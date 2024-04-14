// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"

#include "TestWindowskinThumbnailRenderer.generated.h"

class UWindowskin;

UCLASS()
class POKEMONTESTS_API ATestWindowskinThumbnailRenderer : public AFunctionalTest {
    GENERATED_BODY()

  protected:
    void BeginPlay() override;

  private:
    /**
     * Actually run the test
     */
    UFUNCTION()
    void TestRun();

    /**
     * The Windowskin asset to verify what to do when there is a valid texture
     */
    UPROPERTY(EditInstanceOnly, Category = "Test Parameters")
    TObjectPtr<UWindowskin> ValidWindowskin;
};
