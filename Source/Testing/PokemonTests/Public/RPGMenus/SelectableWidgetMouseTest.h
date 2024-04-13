// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"

#include "SelectableWidgetMouseTest.generated.h"

class USelectableOptionTestWidget;
class USelectableWidget;
class UCommandWindow;

UCLASS()
class POKEMONTESTS_API ASelectableWidgetMouseTest : public AFunctionalTest {
    GENERATED_BODY()

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

  private:
    /**
     * Actually run the test
     */
    UFUNCTION()
    void TestRun();

    UFUNCTION()
    void HandledConfirm(int32 Index);

    UPROPERTY(EditInstanceOnly, Category = "Test Parameters", meta = (MustImplement = "SelectableOptionTestWidget"))
    TSubclassOf<USelectableOptionTestWidget> WidgetClass;

    TOptional<int32> SelectedIndex;
};
