// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"

#include "MenuActionTest.generated.h"

class UMenuAction;
class UScreen;

/**
 * Test performed on a menu action
 */
UCLASS()
class POKEMONTESTS_API AMenuActionTest : public AFunctionalTest {
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
     * The actual action used for the test
     */
    UPROPERTY(EditInstanceOnly, Instanced, Category = "Test Parameters")
    TObjectPtr<UMenuAction> MenuAction;
};
