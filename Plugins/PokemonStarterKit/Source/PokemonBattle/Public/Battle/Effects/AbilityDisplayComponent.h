// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayAbilitySpecHandle.h"
#include "AbilityDisplayComponent.generated.h"

/**
 * The delegate to signal to the ability it can display and is safe to continue executing.
 */
DECLARE_DELEGATE(FOnGameplayAbilityDisplay);

class UAbilitySystemComponent;

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType)
class UAbilityDisplayComponent : public UInterface {
    GENERATED_BODY()
};

/**
 * Interface that stands in for the ability display functionality of a battle
 */
class POKEMONBATTLE_API IAbilityDisplayComponent {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    /**
     * Queue up an ability to display to the player
     * @param Handle The handle of the ability to add to the queue
     * @param AbilitySystemComponent The ability system component that owns the gameplay ability spec handle
     * @param ContinueDelegate The delegate to call when the ability can continue
     */
    virtual void QueueAbilityToDisplay(FGameplayAbilitySpecHandle Handle, UAbilitySystemComponent* AbilitySystemComponent,
                                       FOnGameplayAbilityDisplay&& ContinueDelegate) = 0;

    /**
     * Is an ability currently being displayed to the player?
     * @return Is an ability being displayed?
     */
    virtual bool IsAbilityDisplaying() const = 0;

};