// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/CancellableAsyncAction.h"
#include "GameplayTagContainer.h"
#include "PrimaryGameLayout.h"
#include "UE5Coro.h"

#include "PushContentToLayerForPlayer.generated.h"

class APlayerController;
class UCommonActivatableWidget;
class UObject;
struct FFrame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPushContentToLayerForPlayerAsyncDelegate, UCommonActivatableWidget *,
                                            UserWidget);

/**
 * @class UPushContentToLayerForPlayer
 * @brief An asynchronous action for pushing content to a specific UI layer for a player.
 *
 * This class is part of the RPGMENUS module and allows for dynamically managing UI layers by pushing
 * specified content to a given layer associated with a player controller. The action can be activated
 * or cancelled, providing flexibility for managing UI state in gameplay.
 */
UCLASS()
class RPGMENUS_API UPushContentToLayerForPlayer : public UCancellableAsyncAction {
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, BlueprintCosmetic,
              meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"))
    static UPushContentToLayerForPlayer *PushContentToLayerForPlayer(
        APlayerController *OwningPlayer,
        UPARAM(meta = (AllowAbstract = false)) TSoftClassPtr<UCommonActivatableWidget> WidgetClass,
        UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName, bool bSuspendInputUntilComplete = true);

    void Activate() override;

    void Cancel() override;

    UWorld *GetWorld() const override;

  private:
    UE5Coro::TCoroutine<UCommonActivatableWidget *> PushToLayer(UE5Coro::TLatentContext<UPrimaryGameLayout> Context);

    UPROPERTY(BlueprintAssignable)
    FPushContentToLayerForPlayerAsyncDelegate AfterPush;

    FGameplayTag LayerName;
    bool bSuspendInputUntilComplete = false;
    TWeakObjectPtr<APlayerController> OwningPlayerPtr;
    TSoftClassPtr<UCommonActivatableWidget> WidgetClass;

    TOptional<UE5Coro::TCoroutine<UCommonActivatableWidget *>> Coroutine;
};
