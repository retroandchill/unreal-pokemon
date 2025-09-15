// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/CancellableAsyncAction.h"
#include "GameplayTagContainer.h"
#include "PrimaryGameLayout.h"
#include "UE5Coro.h"

#include "PushWidgetToLayerAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPushContentToLayerAsyncDelegate, UCommonActivatableWidget*, UserWidget);

/**
 * 
 */
UCLASS(BlueprintType)
class RPGMENUS_API UPushWidgetToLayerAsyncAction : public UCancellableAsyncAction
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintCosmetic, BlueprintInternalUseOnly, meta=(DefaultToSelf = "OwningPlayer", DeterminesOutputType = "InWidgetClass", DynamicOutputParam = "UserWidget"))
    static UPushWidgetToLayerAsyncAction* PushWidgetToLayerAsync(APlayerController* OwningPlayer, UPARAM(meta = (AllowAbstract=false)) TSoftClassPtr<UCommonActivatableWidget> InWidgetClass, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag InLayerName, bool bSuspendInputUntilComplete = true);
    
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
