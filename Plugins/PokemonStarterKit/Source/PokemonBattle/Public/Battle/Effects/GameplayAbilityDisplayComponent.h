// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AbilityDisplayComponent.h"
#include "Components/ActorComponent.h"
#include "GameplayAbilityDisplayComponent.generated.h"

class UAbilitySystemComponent;

/**
 * Struct that contains the information needed for an ability callback
 */
struct POKEMONBATTLE_API FAbilityDisplay {

    /**
     * The ability to display
     */
    FGameplayAbilitySpecHandle Ability;

    /**
     * The ability system component that holds the ability
     */
    TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    /**
     * The delegate to call to signal that we can continue
     */
    FOnGameplayAbilityDisplay ContinueDelegate;

    /**
     * Flag to tell if this ability has been called yet
     */
    bool bCalled = false;

    /**
     * Default constructor
     */
    FAbilityDisplay() = default;

    /**
     * Create a new entry with the provided information
     * @param AbilitySpecHandle The ability to display
     * @param AbilitySystemComponent The ability system component that holds the ability
     * @param ContinueDelegate The delegate to call to signal that we can continue
     */
    FAbilityDisplay(FGameplayAbilitySpecHandle AbilitySpecHandle, UAbilitySystemComponent* AbilitySystemComponent, FOnGameplayAbilityDisplay&& ContinueDelegate);
    
};

/**
 * Actor component used in battle for displaying gameplay abilities to the player
 */
UCLASS(ClassGroup=(Battle), meta=(BlueprintSpawnableComponent))
class POKEMONBATTLE_API UGameplayAbilityDisplayComponent : public UActorComponent, public IAbilityDisplayComponent {
    GENERATED_BODY()

public:
    void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
    void QueueAbilityToDisplay(FGameplayAbilitySpecHandle Handle, UAbilitySystemComponent* AbilitySystemComponent, FOnGameplayAbilityDisplay &&ContinueDelegate) override;
    bool IsAbilityDisplaying() const override;
    FDelegateHandle BindToDisplayQueueEmpty(FDisplayQueueEmpty::FDelegate&& Delegate) override;
    void RemoveQueueEmptyBinding(FDelegateHandle DelegateHandle) override;

private:
    /**
     * Process the contents of the ability queue
     */
    void ProcessAbilitiesQueue();
    
    /**
     * The queue of callbacks to apply to the ability in question
     */
    TQueue<FAbilityDisplay> AbilitiesToDisplay;

    /**
     * Called when the queue is empty
     */
    FDisplayQueueEmpty OnQueueEmpty;

};