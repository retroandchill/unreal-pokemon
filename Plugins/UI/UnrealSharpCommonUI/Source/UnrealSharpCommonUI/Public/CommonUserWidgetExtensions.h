// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Input/UIActionBindingHandle.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "CommonUserWidgetExtensions.generated.h"

class UCommonUserWidget;
struct FCSBindUIActionArgs;

/**
 * @class UCommonUserWidgetExtensions
 * @brief A utility class extending functionality for UCommonUserWidget.
 *
 * UCommonUserWidgetExtensions provides several static methods to manage UI action bindings, allowing for
 * registration, retrieval, addition, and removal of action bindings for widgets derived from UCommonUserWidget.
 * This class acts as a BlueprintFunctionLibrary to simplify interaction with UI action binding operations.
 */
UCLASS(meta = (InternalType))
class UNREALSHARPCOMMONUI_API UCommonUserWidgetExtensions : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * @brief Retrieves all action bindings associated with the specified UCommonUserWidget.
     *
     * This method provides access to the list of user interface action bindings registered for
     * the provided widget. Action bindings enable the widget to respond to specific user input actions.
     *
     * @param Widget A pointer to the UCommonUserWidget for which action bindings are to be retrieved.
     *               Must not be null.
     * @return A TArray of FUIActionBindingHandle, representing the action bindings registered to the widget.
     */
    UFUNCTION(meta = (ScriptMethod))
    static TArray<FUIActionBindingHandle> GetActionBindings(const UCommonUserWidget *Widget);

    /**
     * @brief Registers a UI action binding to the specified widget.
     *
     * This method binds a UI action, as specified in the provided action arguments, to the given UCommonUserWidget.
     * Action bindings allow the widget to respond to user input actions dynamically based on the provided
     * configuration.
     *
     * @param Widget A pointer to the UCommonUserWidget to which the action binding will be registered. Must not be
     * null.
     * @param BindActionArgs A struct containing configuration details for the UI action to be registered.
     * @return A FUIActionBindingHandle representing the handle for the newly registered action binding.
     */
    UFUNCTION(meta = (ScriptMethod))
    static FUIActionBindingHandle RegisterUIActionBinding(UCommonUserWidget *Widget,
                                                          const FCSBindUIActionArgs &BindActionArgs);

    /**
     * @brief Adds a UI action binding to the specified widget.
     *
     * This method registers a provided UI action binding to the given UCommonUserWidget,
     * enabling it to handle input actions as defined by the action binding configuration.
     *
     * @param Widget A pointer to the UCommonUserWidget to add the action binding to. Must not be null.
     * @param BindActionArgs An FUIActionBindingHandle containing the details of the action binding to add.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void AddActionBinding(UCommonUserWidget *Widget, FUIActionBindingHandle BindActionArgs);

    /**
     * @brief Removes a UI action binding from the specified widget.
     *
     * This method removes a previously registered UI action binding from the given UCommonUserWidget,
     * disabling the widget's ability to respond to the input action associated with the specified binding handle.
     *
     * @param Widget A pointer to the UCommonUserWidget from which the action binding will be removed. Must not be null.
     * @param BindActionArgs A FUIActionBindingHandle representing the action binding to be removed from the widget.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void RemoveActionBinding(UCommonUserWidget *Widget, FUIActionBindingHandle BindActionArgs);
};
