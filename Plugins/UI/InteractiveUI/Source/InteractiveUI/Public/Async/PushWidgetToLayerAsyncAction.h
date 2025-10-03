// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/CancellableAsyncAction.h"
#include "GameplayTagContainer.h"
#include "PrimaryGameLayout.h"

#include "PushWidgetToLayerAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPushContentToLayerAsyncDelegate, UCommonActivatableWidget *, UserWidget);

/**
 * @class UPushWidgetToLayerAsyncAction
 * @brief An asynchronous action used to push a widget to a specified layer in the UI system.
 *
 * This class allows you to asynchronously push a widget to a specific UI layer
 * and handle events during widget configuration and completion.
 * It supports customizable input suspension until the operation is complete.
 *
 * The class extends UCancellableAsyncAction, enabling cancellation of the operation.
 */
UCLASS(BlueprintType)
class INTERACTIVEUI_API UPushWidgetToLayerAsyncAction : public UCancellableAsyncAction
{
    GENERATED_BODY()

  public:
    /**
     * @brief Initiates an asynchronous operation to push a widget to a specific UI layer.
     *
     * This method creates an asynchronous action that manages the process of adding
     * a widget to a specified layer in the UI. Input can be optionally suspended
     * during the operation until it is completed. It also ensures proper configuration
     * in coordination with the owning player and the specified gameplay tag layer.
     *
     * @param OwningPlayer The player controller that owns the widget being pushed.
     * @param InWidgetClass The class of the widget to be pushed. Must not be null.
     * @param InLayerName The identifier of the UI layer where the widget should be added.
     * @param bSuspendInputUntilComplete If true, input will be suspended until the operation is finished.
     * @return An instance of UPushWidgetToLayerAsyncAction to manage the asynchronous operation,
     *         or nullptr if the operation could not be initiated.
     */
    UFUNCTION(BlueprintCallable, BlueprintCosmetic, BlueprintInternalUseOnly,
              meta = (DefaultToSelf = "OwningPlayer", DeterminesOutputType = "InWidgetClass",
                      DynamicOutputParam = "UserWidget"))
    static UPushWidgetToLayerAsyncAction *PushWidgetToLayerAsync(
        APlayerController *OwningPlayer,
        UPARAM(meta = (AllowAbstract = false)) TSoftClassPtr<UCommonActivatableWidget> InWidgetClass,
        UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag InLayerName, bool bSuspendInputUntilComplete = true);

    void Activate() override;
    void Cancel() override;

  private:
    UPROPERTY(BlueprintAssignable)
    FPushContentToLayerAsyncDelegate ConfigureWidget;

    UPROPERTY(BlueprintAssignable)
    FPushContentToLayerAsyncDelegate OnComplete;

    FGameplayTag LayerName;
    bool bSuspendInputUntilComplete = false;
    TWeakObjectPtr<APlayerController> OwningPlayerPtr;
    TSoftClassPtr<UCommonActivatableWidget> WidgetClass;

    TSharedPtr<FStreamableHandle> StreamingHandle;
};
