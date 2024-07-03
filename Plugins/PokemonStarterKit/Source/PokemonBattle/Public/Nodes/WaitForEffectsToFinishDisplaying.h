// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Effects/AbilityDisplayComponent.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitForEffectsToFinishDisplaying.generated.h"

class UGameplayAbility;

/**
 * Callback for when display is finished
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisplayFinished);

/**
 * Wait for any queued effects on a battler component to finish displaying
 */
UCLASS(meta = (HideThen))
class POKEMONBATTLE_API UWaitForEffectsToFinishDisplaying : public UBlueprintAsyncActionBase {
    GENERATED_BODY()

public:
    /**
     * Wait for any queued effects on a battler component to finish displaying
     * @param Ability The ability to get the battle from
     * @return The created node
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Battle|Display", meta = (DefaultToSelf = Ability))
    static UWaitForEffectsToFinishDisplaying *WaitForEffectsToFinishDisplaying(UGameplayAbility* Ability);

    void Activate() override;
    
private:
    /**
     * Execute the process called the queue is empty
     */
    void ExecuteOnDisplayFinished(TScriptInterface<IAbilityDisplayComponent> AbilityDisplayComponent) const;
    
    /**
     * Called when display is completed
     */
    UPROPERTY(BlueprintAssignable)
    FOnDisplayFinished OnDisplayFinished;
    
    /**
     * The ability to display for
     */
    UPROPERTY()
    TObjectPtr<UGameplayAbility> GameplayAbility;

    /**
     * Delegate handle obtained when this node is activated
     */
    FDelegateHandle DelegateHandle;
};
