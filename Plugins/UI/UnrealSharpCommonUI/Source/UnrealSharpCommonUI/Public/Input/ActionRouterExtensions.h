// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Input/UIActionBindingHandle.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "ActionRouterExtensions.generated.h"

class UCommonActivatableWidget;
class UCommonUIActionRouterBase;
struct FCSBindUIActionArgs;

/**
 * @class UActionRouterExtensions
 * @brief Utility class for extending action routing functionalities in Unreal Engine.
 *
 * UActionRouterExtensions provides a collection of static methods
 * that can be used to facilitate UI action binding through blueprints.
 * This class is derived from UBlueprintFunctionLibrary and is meant
 * to offer additional scripting-related functionalities.
 */
UCLASS()
class UNREALSHARPCOMMONUI_API UActionRouterExtensions : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * @brief Registers a UI action binding to the specified action router.
     *
     * This method creates an action binding to enable UI interaction with a specific widget,
     * using the parameters provided in the binding arguments.
     *
     * @param ActionRouter The action router responsible for handling the binding.
     * @param Widget The widget to which the UI action binding is associated.
     * @param BindActionArgs The arguments that define the parameters for the action binding.
     * @return A handle to the newly created UI action binding.
     */
    UFUNCTION(meta = (ScriptMethod))
    static FUIActionBindingHandle RegisterUIActionBinding(UCommonUIActionRouterBase *ActionRouter,
                                                          const UWidget *Widget,
                                                          const FCSBindUIActionArgs &BindActionArgs);
};
