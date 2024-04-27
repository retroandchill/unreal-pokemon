// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "WidgetTestUtilities.generated.h"

class UWidget;
/**
 * Blueprint functions for getting widget children
 */
UCLASS()
class AUTOMATIONTESTHELPERS_API UWidgetTestUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Find a child widget by its name
     * @param Parent The parent widget
     * @param WidgetName The name of said widget
     * @return The found widget
     */
    UFUNCTION(BlueprintPure, Category = Widgets)
    static UWidget *FindChildWidget(UUserWidget *Parent, FName WidgetName);
    
};
