// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "UIActionBindingExtensions.generated.h"

class UWidget;
struct FUIActionBindingHandle;

/**
 * @class UUIActionBindingExtensions
 * @brief A utility class that provides extension methods for handling UI action bindings.
 *
 * This class extends functionality for UI action bindings, offering methods to query the validity of bindings,
 * manage display properties, reset hold status, and retrieve or set various properties associated with action bindings.
 * Intended to be used as a helper library for manipulating bindings through blueprints or code.
 */
UCLASS(meta = (InternalType))
class UNREALSHARPCOMMONUI_API UUIActionBindingExtensions : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * @brief Determines whether a given UI action binding handle is valid.
     *
     * This method checks if the specified FUIActionBindingHandle is valid and usable.
     *
     * @param Handle The UI action binding handle to validate.
     * @return true if the handle is valid; otherwise, false.
     */
    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static bool IsValid(FUIActionBindingHandle Handle);

    /**
     * @brief Unregisters the specified UI action binding handle.
     *
     * This method removes the specified UI action binding from the system, effectively deactivating
     * the associated action and removing any ties to the binding handle.
     *
     * @param Handle The UI action binding handle to unregister.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void Unregister(FUIActionBindingHandle Handle);

    /**
     * @brief Resets the hold progress for a UI action binding.
     *
     * This method resets the hold progress of the action binding associated with the provided handle to zero.
     * Use this to clear the hold state when resetting or reinitializing the binding's behavior.
     *
     * @param Handle The UI action binding handle whose hold progress should be reset.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void ResetHold(FUIActionBindingHandle Handle);

    /**
     * @brief Retrieves the action name associated with the specified UI action binding handle.
     *
     * This method returns the name of the action bound to the provided FUIActionBindingHandle.
     * It can be used to identify the action associated with the binding for display or logic purposes.
     *
     * @param Handle The UI action binding handle whose action name is to be retrieved.
     * @return The name of the action associated with the given handle.
     */
    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static FName GetActionName(FUIActionBindingHandle Handle);

    /**
     * @brief Retrieves the display name associated with the specified UI action binding handle.
     *
     * This method fetches the display name of the action binding represented by the provided FUIActionBindingHandle.
     * It can be utilized for displaying the name in UI or debugging purposes.
     *
     * @param Handle The UI action binding handle whose display name needs to be retrieved.
     * @return The display name of the action associated with the given handle.
     */
    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static FText GetDisplayName(FUIActionBindingHandle Handle);

    /**
     * @brief Sets the display name for a UI action binding.
     *
     * This method assigns a new display name to the action binding represented by the provided FUIActionBindingHandle.
     * The display name can be used for identifying the action in the UI or other user-facing elements.
     *
     * @param Handle The UI action binding handle for which the display name is to be set.
     * @param DisplayName The new display name to associate with the provided binding handle.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void SetDisplayName(FUIActionBindingHandle Handle, const FText &DisplayName);

    /**
     * @brief Retrieves the display status of an action binding for visibility in the action bar.
     *
     * This method checks whether the specified UI action binding, represented by the given handle,
     * is configured to be displayed in the action bar.
     *
     * @param Handle The UI action binding handle whose display status is to be determined.
     * @return true if the action binding should be displayed in the action bar; otherwise, false.
     */
    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static bool GetDisplayInActionBar(FUIActionBindingHandle Handle);

    /**
     * @brief Sets whether the specified UI action binding is displayed in the action bar.
     *
     * This method updates the display status of a given UI action binding, enabling or disabling its
     * visibility in the action bar depending on the provided parameter.
     *
     * @param Handle The UI action binding handle for which the display status needs to be set.
     * @param bDisplayInActionBar A boolean value indicating whether the action binding should be displayed in the
     * action bar.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void SetDisplayInActionBar(FUIActionBindingHandle Handle, const bool bDisplayInActionBar);

    /**
     * @brief Retrieves the widget bound to the specified UI action binding handle.
     *
     * This method returns the widget currently associated with the provided FUIActionBindingHandle,
     * allowing access to the UI widget tied to the action binding for further interaction or querying.
     *
     * @param Handle The UI action binding handle whose bound widget is to be retrieved.
     * @return A pointer to the widget bound to the specified handle, or nullptr if no widget is bound.
     */
    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static const UWidget *GetBoundWidget(FUIActionBindingHandle Handle);

    /**
     * @brief Retrieves the local player bound to the specified action binding handle.
     *
     * This function allows querying the local player associated with a specific UI action binding,
     * enabling further customization or interaction tied to that player.
     *
     * @param Handle The action binding handle used to identify the relevant UI action binding.
     * @return The local player associated with the given binding handle, or nullptr if no binding exists.
     */
    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static ULocalPlayer *GetBoundLocalPlayer(FUIActionBindingHandle Handle);
};
