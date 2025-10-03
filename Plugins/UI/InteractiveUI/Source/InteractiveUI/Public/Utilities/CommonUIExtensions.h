// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "CommonUIExtensions.generated.h"

class UCommonActivatableWidget;

/**
 * UCommonUIExtensions is a static utility class derived from UBlueprintFunctionLibrary
 * that provides various global UI functionalities, primarily to enhance
 * interactivity and manage content interaction in Unreal Engine.
 */
UCLASS()
class INTERACTIVEUI_API UCommonUIExtensions : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * GetOwningPlayerInputType is a static function that retrieves the input type
     * associated with the player owning the specified UUserWidget.
     *
     * This function utilizes chaining with optional pointers to safely access the
     * input subsystem of the player's owning context and determine the current
     * input type, such as gamepad or keyboard/mouse. If the input type cannot be
     * resolved, a default of ECommonInputType::Count is returned.
     *
     * @param WidgetContextObject A pointer to the UUserWidget that provides the
     * context for determining the owning player's input type.
     * @return The input type (ECommonInputType) of the owning player if available,
     * or ECommonInputType::Count as a fallback default.
     */
    UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "Global UI Extensions",
              meta = (WorldContext = "WidgetContextObject", ExtensionMethod))
    static ECommonInputType GetOwningPlayerInputType(const UUserWidget *WidgetContextObject);

    /**
     * Determines if the owning player of the specified UUserWidget is currently using touch inputs.
     *
     * This static function checks the input type associated with the player owning
     * the given widget context and returns true if the input type is touch.
     *
     * @param WidgetContextObject A pointer to the UUserWidget that serves as the context
     * for determining the owning player's input type.
     * @return True if the owning player's input type is touch, otherwise false.
     */
    UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "Global UI Extensions",
              meta = (WorldContext = "WidgetContextObject", ExtensionMethod))
    static bool IsOwningPlayerUsingTouch(const UUserWidget *WidgetContextObject);

    /**
     * Determines if the owning player of the specified UUserWidget is currently using a gamepad.
     *
     * This static function evaluates the input type associated with the player owning
     * the provided widget context and returns true if the input type is a gamepad.
     *
     * @param WidgetContextObject A pointer to the UUserWidget that serves as the context
     * for determining the owning player's input type.
     * @return True if the owning player's input type is a gamepad, otherwise false.
     */
    UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "Global UI Extensions",
              meta = (WorldContext = "WidgetContextObject", ExtensionMethod))
    static bool IsOwningPlayerUsingGamepad(const UUserWidget *WidgetContextObject);

    /**
     * Pushes a widget of the specified class to a designated layer stack within the UI system associated
     * with the given local player. This function ensures the local player and widget class are valid before
     * proceeding to interact with the UI subsystems for adding content to a layer.
     *
     * @param LocalPlayer The local player whose UI layers will be affected. Must not be null.
     * @param LayerName A gameplay tag representing the name of the layer to which the widget will be pushed.
     * @param WidgetClass The class of the widget to instantiate and push to the specified layer. Must not be null.
     * @return A pointer to the newly created widget if successful, or nullptr if the operation fails.
     */
    UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions",
              meta = (DeterminesOutputType = "WidgetClass", DynamicOutputParam = "ReturnValue", ExtensionMethod))
    static UCommonActivatableWidget *PushContentToLayer(
        const ULocalPlayer *LocalPlayer, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName,
        UPARAM(meta = (AllowAbstract = false)) TSubclassOf<UCommonActivatableWidget> WidgetClass);

    /**
     * Pushes a widget of the specified class to a designated layer stack within the UI system
     * associated with the provided local player. This function validates the input parameters
     * and utilizes the UI manager subsystem to add the widget to the specified layer asynchronously.
     *
     * @param LocalPlayer The local player whose UI layer stack will be modified. Must not be null.
     * @param LayerName A gameplay tag that identifies the layer to which the widget will be added.
     * @param WidgetClass A soft class pointer referencing the widget class to instantiate and push
     * to the specified layer. Must not be null.
     */
    UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions", meta = (ExtensionMethod))
    static void PushStreamedContentToLayer(
        const ULocalPlayer *LocalPlayer, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName,
        UPARAM(meta = (AllowAbstract = false)) TSoftClassPtr<UCommonActivatableWidget> WidgetClass);

    /**
     * Removes the specified activatable widget from its associated UI layer,
     * ensuring that it is no longer displayed or interacted with.
     *
     * @param ActivatableWidget The widget to be removed from its UI layer. If the widget is null,
     *                          the operation is ignored.
     */
    UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions", meta = (ExtensionMethod))
    static void PopContentFromLayer(UCommonActivatableWidget *ActivatableWidget);

    /**
     * Retrieves the ULocalPlayer instance associated with the specified APlayerController.
     * This method determines whether the provided PlayerController is valid and, if so,
     * retrieves the corresponding ULocalPlayer. If the PlayerController is null, it will return nullptr.
     *
     * @param PlayerController A pointer to the APlayerController from which the ULocalPlayer is to be retrieved.
     * @return A pointer to the ULocalPlayer associated with the provided PlayerController, or nullptr if the
     * PlayerController is invalid.
     */
    UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions", meta = (ExtensionMethod))
    static ULocalPlayer *GetLocalPlayerFromController(APlayerController *PlayerController);

    /**
     * Suspends input for the specified player using the given suspend reason.
     * This function is often used to temporarily block user input for specific contexts
     * or scenarios within the UI framework of Unreal Engine.
     *
     * @param PlayerController The player controller representing the target player whose input will be suspended.
     * @param SuspendReason A name representing the reason for suspending input, typically used for tracking purposes.
     * @return The name associated with the suspend reason, confirming the action taken.
     */
    UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions", meta = (ExtensionMethod))
    static FName SuspendInputForPlayer(APlayerController *PlayerController, FName SuspendReason);

    /**
     * Suspends input for the specified player by applying filters for different input types
     * (MouseAndKeyboard, Gamepad, and Touch) and associates the suspension with a unique token.
     *
     * @param LocalPlayer The local player instance for which the input suspension is applied.
     * @param SuspendReason A unique identifier or reason describing the purpose of the input suspension.
     * @return A token representing the input suspension, which can be used to track or revoke the suspension later.
     *         Returns NAME_None if the input subsystem is not available.
     */
    static FName SuspendInputForPlayer(const ULocalPlayer *LocalPlayer, FName SuspendReason);

    /**
     * Resumes input handling for the specified player by utilizing the provided suspend token.
     *
     * @param PlayerController The player controller associated with the player whose input is to be resumed. Should not
     * be null.
     * @param SuspendToken The token that represents the input suspension to be lifted.
     */
    UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions", meta = (ExtensionMethod))
    static void ResumeInputForPlayer(APlayerController *PlayerController, FName SuspendToken);

    /**
     * Resumes input handling for the specified player by removing input type filters
     * associated with the given suspend token.
     *
     * @param LocalPlayer A pointer to the local player whose input handling is being resumed.
     * @param SuspendToken The name of the suspend token used to track input suspension.
     *                     If the token is NAME_None, the function exits without action.
     */
    static void ResumeInputForPlayer(const ULocalPlayer *LocalPlayer, FName SuspendToken);

  private:
    static int32 InputSuspensions;
};
