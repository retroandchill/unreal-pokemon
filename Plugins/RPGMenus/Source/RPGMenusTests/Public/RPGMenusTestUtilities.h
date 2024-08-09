// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/SelectableWidget.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "RPGMenusTestUtilities.generated.h"

class UMessageWindow;
/**
 *
 */
UCLASS(DisplayName = "RPG Menus Test Utilities")
class RPGMENUSTESTS_API URPGMenusTestUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, Category = Selection)
    static void SelectCurrentOption(USelectableWidget *SelectableWidget);

    UFUNCTION(BlueprintCallable, Category = Selection)
    static void CancelOnWidget(USelectableWidget *SelectableWidget);

    UFUNCTION(BlueprintCallable, Category = Messages)
    static void ForceAdvanceText(UMessageWindow *MessageWindow, FName InputName = "MenuConfirm");

    UFUNCTION(BlueprintCallable, Category = Message)
    static void ForceClickButton(UCommonButtonBase *Button);
};
