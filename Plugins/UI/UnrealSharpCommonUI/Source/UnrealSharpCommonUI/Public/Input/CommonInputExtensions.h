// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "CommonInputExtensions.generated.h"

class UInputAction;
/**
 * Represents a utility class that provides various extensions for handling common input-related functionality
 * within the Unreal Engine framework. This class serves as a library of static methods for managing enhanced
 * input features and default actions.
 */
UCLASS()
class UNREALSHARPCOMMONUI_API UCommonInputExtensions : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * Determines whether enhanced input support is enabled within the context
     * of the Unreal Engine framework. This method checks for the availability of
     * enhanced input features and returns its status.
     *
     * @return True if enhanced input support is enabled, false otherwise.
     */
    UFUNCTION(meta = (ScriptMethod))
    static bool IsEnhancedInputSupportEnabled();

    /**
     * Retrieves the enhanced input action designated for the 'Back' action
     * within the context of the Unreal Engine framework.
     *
     * @return The UInputAction instance associated with the enhanced input 'Back' action.
     */
    UFUNCTION(meta = (ScriptMethod))
    static UInputAction *GetEnhancedInputBackAction();

    /**
     * Retrieves the default back action as defined in the settings of the
     * Common Input Module. This action is typically used to handle navigation
     * or back functionality within the UI or gameplay context.
     *
     * @return The default back action represented as an FDataTableRowHandle.
     */
    UFUNCTION(meta = (ScriptMethod))
    static FDataTableRowHandle GetDefaultBackAction();
};
