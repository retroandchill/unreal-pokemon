// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FieldItemEffect.generated.h"

/**
 * Called when the item is done evaluating.
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemEffectComplete, bool);

/**
 * Basic abstract definition of an item effect, regardless of usage type.
 */
UCLASS(Abstract, Blueprintable)
class POKEMONITEMS_API UFieldItemEffect : public UObject {
    GENERATED_BODY()

public:
    UWorld* GetWorld() const override;
    
    /**
     * Bind a delegate to the callback when an item is done.
     * @param Delegate The callback for when the item has resolved.
     * @return The handle to the delegate in question.
     */
    FDelegateHandle BindToEffectComplete(FOnItemEffectComplete::FDelegate&& Delegate);

protected:
    /**
     * Resolve the item effect, and consume if it was successful.
     * @param bSuccess Was the item successfully used.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Items)
    void EffectComplete(bool bSuccess) const;

private:
    FOnItemEffectComplete OnEffectComplete;

};
