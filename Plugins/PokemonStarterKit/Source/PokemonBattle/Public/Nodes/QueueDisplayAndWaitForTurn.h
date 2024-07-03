// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "QueueDisplayAndWaitForTurn.generated.h"

class UGameplayAbility;
class IAbilityDisplayComponent;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnToDisplay);

/**
 * Queue a gameplay ability display and wait for the Ability Display Component to queue up this actor
 */
UCLASS(meta = (HideThen))
class POKEMONBATTLE_API UQueueDisplayAndWaitForTurn : public UBlueprintAsyncActionBase {
    GENERATED_BODY()

public:
    /**
     * Queue a gameplay ability display and wait for the Ability Display Component to queue up this actor
     * @param Ability The gameplay ability that is being displayed
     * @return The created node
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Battle|Display", meta = (DefaultToSelf = Ability))
    static UQueueDisplayAndWaitForTurn *QueueDisplayAndWaitForTurn(UGameplayAbility* Ability);

    void Activate() override;
    
private:
    /**
     * Perform a search for the ability display component
     * @return The found component
     */
    TScriptInterface<IAbilityDisplayComponent> FindAbilityDisplayComponent() const;

    /**
     * Execute the process called when it's the specified ability's turn
     */
    void ExecuteOnTurn() const;

    /**
     * Called when its the ability's turn to display
     */
    UPROPERTY(BlueprintAssignable)
    FOnTurnToDisplay OnTurnToDisplay;
    
    /**
     * The ability to display for
     */
    UPROPERTY()
    TObjectPtr<UGameplayAbility> GameplayAbility;

};
