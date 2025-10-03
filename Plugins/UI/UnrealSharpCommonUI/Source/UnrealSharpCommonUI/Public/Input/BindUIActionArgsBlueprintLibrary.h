// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputModeTypes.h"
#include "CommonInputTypeEnum.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UITag.h"

#include "BindUIActionArgsBlueprintLibrary.generated.h"

class UInputAction;
struct FCSBindUIActionArgs;

DECLARE_DYNAMIC_DELEGATE(FOnExecuteActionDelegate);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnHoldActionProgressedDelegate, float, Progress);
DECLARE_DYNAMIC_DELEGATE(FOnHoldActionPressedDelegate);
DECLARE_DYNAMIC_DELEGATE(FOnHoldActionReleasedDelegate);

/**
 * @class UBindUIActionArgsBlueprintLibrary
 * @brief A utility class providing static library functions to manage and manipulate UI action argument structures.
 *
 * This class contains a collection of helper functions to initialize, check, and change properties
 * of `FCSBindUIActionArgs` structures. It is designed to work with UI action handling in Unreal Engine,
 * including the use of input actions, action tags, and other customizable properties.
 */
UCLASS(meta = (InternalType))
class UNREALSHARPCOMMONUI_API UBindUIActionArgsBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * @brief Validates the given `FCSBindUIActionArgs` structure.
     *
     * This function checks the validity of the provided `FCSBindUIActionArgs` instance,
     * ensuring that it satisfies any required conditions for proper use in UI action handling.
     *
     * @param Args The `FCSBindUIActionArgs` instance to validate.
     * @return True if the given `FCSBindUIActionArgs` is valid; otherwise, false.
     */
    UFUNCTION(meta = (ScriptMethod))
    static bool IsValid(const FCSBindUIActionArgs &Args);

    /**
     * @brief Initializes the action tag and associated execution delegate for a given `FCSBindUIActionArgs` structure.
     *
     * This function sets the action tag and assigns an execution delegate to the provided `FCSBindUIActionArgs`
     * instance, facilitating proper configuration for UI action handling.
     *
     * @param Arg A reference to the `FCSBindUIActionArgs` structure to initialize.
     * @param InActionTag The action tag to associate with the `FCSBindUIActionArgs` structure.
     * @param InOnExecuteAction The delegate to be executed when the action is triggered.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void InitActionTag(UPARAM(ref) FCSBindUIActionArgs &Arg, FUIActionTag InActionTag,
                              FOnExecuteActionDelegate InOnExecuteAction);

    /**
     * @brief Initializes the action tag, display flag, and execution delegate for a given `FCSBindUIActionArgs`
     * structure.
     *
     * This function configures a `FCSBindUIActionArgs` instance by assigning the specified action tag,
     * setting whether it should display in the action bar, and associating an execution delegate for handling the
     * action.
     *
     * @param Arg A reference to the `FCSBindUIActionArgs` structure to be initialized.
     * @param InActionTag The action tag to be associated with the `FCSBindUIActionArgs` structure.
     * @param bShouldDisplayInActionBar A boolean indicating whether the action should be displayed in the action bar.
     * @param InOnExecuteAction The delegate to be executed when the action is triggered.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void InitActionTagBool(UPARAM(ref) FCSBindUIActionArgs &Arg, FUIActionTag InActionTag,
                                  bool bShouldDisplayInActionBar, FOnExecuteActionDelegate InOnExecuteAction);

    /**
     * @brief Initializes a `FCSBindUIActionArgs` structure with a data table row handle and an execution delegate.
     *
     * This function configures a given `FCSBindUIActionArgs` instance by assigning the specified
     * data table row handle and associating an execution delegate responsible for handling the action.
     *
     * @param Arg A reference to the `FCSBindUIActionArgs` structure to initialize.
     * @param InActionTableRow The data table row handle to be associated with the `FCSBindUIActionArgs` structure.
     * @param InOnExecuteAction The delegate to be executed when the action specified by the row handle is triggered.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void InitDataTableRowHandle(UPARAM(ref) FCSBindUIActionArgs &Arg,
                                       const FDataTableRowHandle &InActionTableRow,
                                       FOnExecuteActionDelegate InOnExecuteAction);

    /**
     * @brief Initializes a `FCSBindUIActionArgs` structure with a data table row handle, display flag, and an execution
     * delegate.
     *
     * This function sets up a `FCSBindUIActionArgs` instance by associating it with the specified data table row
     * handle, setting whether it should be displayed in the action bar, and assigning an execution delegate to handle
     * the action.
     *
     * @param Arg A reference to the `FCSBindUIActionArgs` structure to initialize.
     * @param InActionTableRow The data table row handle to be associated with the `FCSBindUIActionArgs` structure.
     * @param bShouldDisplayInActionBar A boolean indicating whether the action should appear in the action bar.
     * @param InOnExecuteAction The delegate to be executed when the action specified by the row handle is triggered.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void InitDataTableRowHandleBool(UPARAM(ref) FCSBindUIActionArgs &Arg,
                                           const FDataTableRowHandle &InActionTableRow, bool bShouldDisplayInActionBar,
                                           FOnExecuteActionDelegate InOnExecuteAction);

    /**
     * @brief Initializes an `FCSBindUIActionArgs` structure with an input action and a delegate for execution.
     *
     * This function sets up the specified `FCSBindUIActionArgs` structure with the provided input action and
     * execution delegate. It allows configuring UI action arguments for input handling in Unreal Engine.
     *
     * @param Arg A reference to the `FCSBindUIActionArgs` structure to be initialized.
     * @param InInputAction A pointer to the `UInputAction` used to bind the UI action.
     * @param InOnExecuteAction A delegate specifying the function to execute when the action is triggered.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void InitInputAction(UPARAM(ref) FCSBindUIActionArgs &Arg, const UInputAction *InInputAction,
                                FOnExecuteActionDelegate InOnExecuteAction);

    /**
     * @brief Initializes an `FCSBindUIActionArgs` structure with specific input action and behavior settings.
     *
     * This method sets up an `FCSBindUIActionArgs` object with a given input action, an action bar visibility flag,
     * and a delegate to execute when the action occurs.
     *
     * @param Arg The `FCSBindUIActionArgs` structure to be initialized.
     * @param InInputAction The input action object to associate with the UI action arguments.
     * @param bShouldDisplayInActionBar A boolean flag indicating whether the input action should be displayed in the
     * action bar.
     * @param InOnExecuteAction A delegate that specifies the action to execute when the associated input action is
     * triggered.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void InitInputActionBool(UPARAM(ref) FCSBindUIActionArgs &Arg, const UInputAction *InInputAction,
                                    bool bShouldDisplayInActionBar, FOnExecuteActionDelegate InOnExecuteAction);

    /**
     * @fn FCSBindUIActionArgs UBindUIActionArgsBlueprintLibrary::Clone(const FCSBindUIActionArgs &Arg)
     * @brief Creates a copy of the provided `FCSBindUIActionArgs` structure.
     *
     * This function accepts a reference to an `FCSBindUIActionArgs` structure and returns a new instance of the same.
     * If the provided argument is valid, it clones the internal properties of the structure.
     *
     * @param Arg The `FCSBindUIActionArgs` structure to be cloned.
     * @return A new `FCSBindUIActionArgs` structure containing the cloned data, or the original input if the argument
     * is invalid.
     */
    UFUNCTION(meta = (ScriptMethod))
    static FCSBindUIActionArgs Clone(const FCSBindUIActionArgs &Arg);

    /**
     * @brief Retrieves the name of the action from the provided UI action argument structure.
     *
     * This function extracts the action name from the given `FCSBindUIActionArgs` structure to be
     * used in UI action handling workflows.
     *
     * @param Arg The `FCSBindUIActionArgs` structure containing the information related to the UI action.
     * @return The name of the action as a `FName` extracted from the provided argument.
     */
    UFUNCTION(meta = (ScriptMethod))
    static FName GetActionName(const FCSBindUIActionArgs &Arg);

    /**
     * @brief Checks if the specified UI action arguments include hold mappings.
     *
     * This function evaluates the given `FCSBindUIActionArgs` object to determine whether hold mappings
     * are associated with the defined UI action. Hold mappings typically refer to inputs that require
     * the user to hold an action instead of a simple press.
     *
     * @param Arg The `FCSBindUIActionArgs` object representing the UI action arguments to be checked.
     * @return Returns true if the UI action includes hold mappings, otherwise returns false.
     */
    UFUNCTION(meta = (ScriptMethod))
    static bool ActionHasHoldMappings(const FCSBindUIActionArgs &Arg);

    /**
     * @brief Retrieves the action tag from the specified UI action argument.
     *
     * This function extracts and returns the action tag assigned to a given `FCSBindUIActionArgs` structure.
     *
     * @param Arg The UI action argument structure containing the action tag to retrieve.
     * @return The action tag associated with the provided UI action argument.
     */
    UFUNCTION(meta = (ScriptMethod))
    static FUIActionTag GetActionTag(const FCSBindUIActionArgs &Arg);

    /**
     * @brief Sets the action tag for the specified UI action argument structure.
     *
     * This function updates the `ActionTag` property of the given `FCSBindUIActionArgs`
     * structure with the specified action tag value.
     *
     * @param Arg The UI action argument structure to be modified.
     * @param InActionTag The new action tag to set for the specified structure.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void SetActionTag(const FCSBindUIActionArgs &Arg, FUIActionTag InActionTag);

    /**
     * @brief Retrieves the legacy action table row associated with the provided UI action arguments.
     *
     * This function is used to access the legacy action table row stored in the `FCSBindUIActionArgs`
     * structure. It allows interaction with the associated data table row handle that defines
     * legacy UI action behavior.
     *
     * @param Arg A reference to the `FCSBindUIActionArgs` structure containing the UI action arguments.
     * @return A constant reference to the `FDataTableRowHandle` representing the legacy action table row.
     */
    UFUNCTION(meta = (ScriptMethod))
    static const FDataTableRowHandle &GetActionTableRow(const FCSBindUIActionArgs &Arg);

    /**
     * @brief Sets the action table row for a given FCSBindUIActionArgs argument.
     *
     * This function updates the legacy action table row property of the provided
     * FCSBindUIActionArgs structure to the specified data table row handle.
     *
     * @param Arg The FCSBindUIActionArgs structure whose action table row will be set.
     * @param InActionTableRow The data table row handle to assign to the action table row.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void SetActionTableRow(const FCSBindUIActionArgs &Arg, const FDataTableRowHandle &InActionTableRow);

    /**
     * @brief Retrieves the input action associated with the given UI action argument.
     *
     * This static method extracts the `UInputAction` object from the provided `FCSBindUIActionArgs` structure.
     * It allows access to the configured input action that defines the behavior and triggers tied
     * to the respective UI action.
     *
     * @param Arg The UI action argument structure from which to retrieve the input action.
     * @return A pointer to the `UInputAction` associated with the given argument.
     */
    UFUNCTION(meta = (ScriptMethod))
    static const UInputAction *GetInputAction(const FCSBindUIActionArgs &Arg);

    /**
     * @brief Sets the input action for a given `FCSBindUIActionArgs` structure.
     *
     * This function allows you to assign an `UInputAction` to the specified UI action argument,
     * enabling interaction with the designated input action in UI handling.
     *
     * @param Arg The `FCSBindUIActionArgs` structure to which the input action will be assigned.
     * @param InInputAction The `UInputAction` to be set in the `FCSBindUIActionArgs` structure.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void SetInputAction(const FCSBindUIActionArgs &Arg, const UInputAction *InInputAction);

    /**
     * @brief Retrieves the input mode from the specified UI action arguments structure.
     *
     * This function extracts and returns the input mode associated with the provided `FCSBindUIActionArgs` structure.
     * The input mode defines the intended interaction model for a particular UI action.
     *
     * @param Arg The `FCSBindUIActionArgs` structure containing the UI action arguments.
     * @return The input mode as an `ECommonInputMode` value.
     */
    UFUNCTION(meta = (ScriptMethod))
    static ECommonInputMode GetInputMode(const FCSBindUIActionArgs &Arg);

    /**
     * @brief Sets the input mode for the specified UI action arguments.
     *
     * This function allows modification of the input mode property in a given `FCSBindUIActionArgs`
     * structure. Input mode dictates how the input system interacts with the UI action.
     *
     * @param Arg The `FCSBindUIActionArgs` structure whose input mode will be set.
     * @param InInputMode The desired input mode to assign, defined by the `ECommonInputMode` enumeration.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void SetInputMode(const FCSBindUIActionArgs &Arg, ECommonInputMode InInputMode);

    /**
     * @brief Retrieves the key event associated with the specified UI action argument.
     *
     * This function returns the `EInputEvent` value that represents the key event
     * type stored in the given `FCSBindUIActionArgs` structure.
     *
     * @param Arg The `FCSBindUIActionArgs` structure containing the key event information.
     * @return The key event (`EInputEvent`) associated with the specified argument.
     */
    UFUNCTION(meta = (ScriptMethod))
    static EInputEvent GetKeyEvent(const FCSBindUIActionArgs &Arg);

    /**
     * @brief Sets the key event for a given UI action argument structure.
     *
     * This function allows customization of the key event property within an `FCSBindUIActionArgs` structure,
     * which is used for handling specific input events in UI action logic.
     *
     * @param Arg The UI action argument structure to modify.
     * @param InKeyEvent The input event to set, defining the key action type (e.g., pressed, released, etc.).
     */
    UFUNCTION(meta = (ScriptMethod))
    static void SetKeyEvent(const FCSBindUIActionArgs &Arg, EInputEvent InKeyEvent);

    /**
     * @brief Retrieves the set of input types exempt from the valid key check within the specified UI action arguments.
     *
     * This function accesses the input types that are excluded from the valid key verification process
     * in a given `FCSBindUIActionArgs` structure. These exempted input types are used to allow specific
     * inputs without triggering a validity check during UI interaction handling.
     *
     * @param Arg The `FCSBindUIActionArgs` structure containing the data to extract the exempt input types from.
     * @return A set of `ECommonInputType` values representing the input types that are exempted from the valid key
     * check.
     */
    UFUNCTION(meta = (ScriptMethod))
    static const TSet<ECommonInputType> &GetInputTypesExemptFromValidKeyCheck(const FCSBindUIActionArgs &Arg);

    /**
     * @brief Sets the input types that are exempt from a valid key check in the provided UI action argument structure.
     *
     * This function modifies the input types that are considered exempt from the valid key check
     * in the supplied `FCSBindUIActionArgs` structure. It is designed to customize the behavior
     * of UI action handling by specifying exceptions for input validation.
     *
     * @param Arg The UI action argument structure to modify.
     * @param InInputTypesExemptFromValidKeyCheck A set of input types to designate as exempt from the valid key check.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void SetInputTypesExemptFromValidKeyCheck(const FCSBindUIActionArgs &Arg,
                                                     TSet<ECommonInputType> InInputTypesExemptFromValidKeyCheck);

    /**
     * @brief Retrieves the persistence status of a given UI action argument.
     *
     * This function checks if the provided `FCSBindUIActionArgs` structure is marked as persistent.
     * Persistence indicates whether the action's configuration should be retained across different
     * contexts or lifetimes.
     *
     * @param Arg The UI action argument to evaluate.
     * @return True if the action argument is persistent, otherwise false.
     */
    UFUNCTION(meta = (ScriptMethod))
    static bool GetIsPersistent(const FCSBindUIActionArgs &Arg);

    /**
     * @brief Sets the persistence property of a UI action argument structure.
     *
     * Use this function to enable or disable the persistence property on a specified `FCSBindUIActionArgs` structure.
     * Persistence determines whether the action remains active across certain context changes.
     *
     * @param Arg The UI action argument structure whose persistence property will be updated.
     * @param bInIsPersistent A boolean value indicating whether the action should be persistent (true) or not (false).
     */
    UFUNCTION(meta = (ScriptMethod))
    static void SetIsPersistent(const FCSBindUIActionArgs &Arg, bool bInIsPersistent);

    /**
     * @brief Retrieves the "consume input" property from a given `FCSBindUIActionArgs` structure.
     *
     * This function provides access to the `bConsumeInput` property, indicating whether the input
     * action captured by the corresponding UI binding should be consumed or not.
     *
     * @param Arg A reference to an `FCSBindUIActionArgs` structure containing the input action arguments.
     * @return A boolean value representing the state of the "consume input" property.
     */
    UFUNCTION(meta = (ScriptMethod))
    static bool GetConsumeInput(const FCSBindUIActionArgs &Arg);

    /**
     * @brief Sets the consume input property for the given UI action arguments structure.
     *
     * This function updates the `bConsumeInput` property of the provided `FCSBindUIActionArgs` structure.
     * It is used to enable or disable input consumption for the associated UI action.
     *
     * @param Arg The UI action arguments structure to be modified.
     * @param bInConsumeInput The boolean value specifying whether to consume input (`true`) or not (`false`).
     */
    UFUNCTION(meta = (ScriptMethod))
    static void SetConsumeInput(const FCSBindUIActionArgs &Arg, bool bInConsumeInput);

    /**
     * @brief Retrieves the visibility setting for displaying the action in the action bar from the provided UI action
     * arguments.
     *
     * This function checks the `FCSBindUIActionArgs` structure to determine if the action is configured
     * to be displayed in the action bar.
     *
     * @param Arg A reference to the `FCSBindUIActionArgs` structure containing the UI action configuration.
     * @return A boolean value indicating whether the action is set to be displayed in the action bar.
     */
    UFUNCTION(meta = (ScriptMethod))
    static bool GetDisplayInActionBar(const FCSBindUIActionArgs &Arg);

    /**
     * @brief Sets the visibility of an action in the action bar.
     *
     * This function modifies the `bDisplayInActionBar` property of the provided `FCSBindUIActionArgs` structure,
     * allowing the action to be displayed or hidden in the action bar.
     *
     * @param Arg The reference to the `FCSBindUIActionArgs` structure whose property is being modified.
     * @param bInDisplayInActionBar A boolean value indicating whether the action should be displayed
     * in the action bar (`true`) or hidden (`false`).
     */
    UFUNCTION(meta = (ScriptMethod))
    static void SetDisplayInActionBar(const FCSBindUIActionArgs &Arg, bool bInDisplayInActionBar);

    /**
     * @brief Retrieves the "Force Hold" property of a UI action argument structure.
     *
     * This function returns whether the "Force Hold" behavior is enabled for a given `FCSBindUIActionArgs` instance.
     * The "Force Hold" property dictates if the associated action must be held down rather than tapped.
     *
     * @param Arg The `FCSBindUIActionArgs` structure containing the action arguments to query.
     * @return A boolean value indicating whether the "Force Hold" property is enabled.
     */
    UFUNCTION(meta = (ScriptMethod))
    static bool GetForceHold(const FCSBindUIActionArgs &Arg);

    /**
     * @brief Sets the ForceHold property for a given UI action argument.
     *
     * This function allows the modification of the `bForceHold` property within the provided `FCSBindUIActionArgs`
     * structure, determining whether the associated UI action should require being held or not.
     *
     * @param Arg The structure containing the UI action arguments to be modified.
     * @param bInForceHold Boolean value specifying whether the ForceHold property should be enabled (true) or disabled
     * (false).
     */
    UFUNCTION(meta = (ScriptMethod))
    static void SetForceHold(const FCSBindUIActionArgs &Arg, bool bInForceHold);

    /**
     * @brief Retrieves the override display name from a given UI action argument structure.
     *
     * This function accesses the `OverrideDisplayName` property of a UI action argument object
     * to provide the associated display name, if any, set as an override for the action.
     *
     * @param Arg The UI action argument structure from which to retrieve the override display name.
     * @return A reference to the override display name associated with the specified argument.
     */
    UFUNCTION(meta = (ScriptMethod))
    static const FText &GetOverrideDisplayName(const FCSBindUIActionArgs &Arg);

    /**
     * @brief Sets an override display name for the given UI action argument structure.
     *
     * This function allows the user to specify a custom display name for the provided
     * `FCSBindUIActionArgs` structure, which can be used in UI displays or other contexts.
     * The provided display name replaces any default or pre-existing display name.
     *
     * @param Arg The UI action argument structure whose override display name is to be set.
     * @param InOverrideDisplayName The custom display name to override the default or existing one.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void SetOverrideDisplayName(const FCSBindUIActionArgs &Arg, FText InOverrideDisplayName);

    /**
     * @brief Retrieves the priority of a given UI action argument within its collection.
     *
     * This static method extracts the `PriorityWithinCollection` property from the specified
     * `FCSBindUIActionArgs` structure. It is used to determine the prioritization of the action
     * within its associated collection of UI actions.
     *
     * @param Arg The argument structure containing the priority information.
     * @return The priority value of the action within the collection.
     */
    UFUNCTION(meta = (ScriptMethod))
    static int32 GetPriorityWithinCollection(const FCSBindUIActionArgs &Arg);

    /**
     * @brief Sets the priority within the collection for a given UI action argument structure.
     *
     * This function allows modifying the priority level of a UI action argument within its collection.
     * Adjusting this value can impact the user's ability to interact with specific actions when multiple
     * actions are present in the same context.
     *
     * @param Arg The `FCSBindUIActionArgs` structure representing the UI action argument to modify.
     * @param InPriorityWithinCollection An integer value specifying the new priority level within the collection.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void SetPriorityWithinCollection(const FCSBindUIActionArgs &Arg, int32 InPriorityWithinCollection);

    /**
     * @brief Sets the delegate to be executed when the action associated with the provided UI action arguments is
     * triggered.
     *
     * This function binds a specified delegate to the `OnExecuteAction` property of the provided `FCSBindUIActionArgs`
     * structure, enabling custom behavior when the UI action is executed.
     *
     * @param Arg The `FCSBindUIActionArgs` structure holding the action's argument properties.
     * @param InOnExecuteAction The delegate to bind to the `OnExecuteAction` property. This delegate will be called
     * when the action is executed.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void SetOnExecuteAction(const FCSBindUIActionArgs &Arg, FOnExecuteActionDelegate InOnExecuteAction);

    /**
     * @brief Sets the delegate for "OnHoldActionProgressed" in the specified UI action arguments structure.
     *
     * This function binds the given delegate to handle the progress of a hold action associated
     * with the specified `FCSBindUIActionArgs` structure.
     *
     * @param Arg The `FCSBindUIActionArgs` structure containing the UI action arguments.
     * @param InOnHoldActionProgressed The delegate to be invoked when the hold action progresses.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void SetOnHoldActionProgressed(const FCSBindUIActionArgs &Arg,
                                          FOnHoldActionProgressedDelegate InOnHoldActionProgressed);

    /**
     * @brief Sets the delegate to be executed when a hold action is pressed within the specified UI action argument.
     *
     * This function binds a custom delegate to the `OnHoldActionPressed` event of the provided
     * `FCSBindUIActionArgs` structure, allowing callbacks to be triggered when the associated hold action is performed.
     *
     * @param Arg The UI action argument structure to which the delegate should be bound.
     * @param InOnHoldActionPressed The delegate to execute when the associated hold action is pressed.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void SetOnHoldActionPressed(const FCSBindUIActionArgs &Arg,
                                       FOnHoldActionPressedDelegate InOnHoldActionPressed);

    /**
     * @brief Sets the delegate to be triggered when a hold action is released for the specified UI action argument
     * structure.
     *
     * This function allows you to bind a custom delegate to the "OnHoldActionReleased" field of a `FCSBindUIActionArgs`
     * structure.
     *
     * @param Arg The `FCSBindUIActionArgs` structure representing the UI action arguments to modify.
     * @param InOnHoldActionReleased The delegate to bind to the "OnHoldActionReleased" event of the provided argument
     * structure.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void SetOnHoldActionReleased(const FCSBindUIActionArgs &Arg,
                                        FOnHoldActionReleasedDelegate InOnHoldActionReleased);
};
