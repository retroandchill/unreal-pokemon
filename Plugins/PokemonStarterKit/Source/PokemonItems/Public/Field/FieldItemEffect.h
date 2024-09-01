// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "FieldItemEffect.generated.h"

struct FItem;
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
#if WITH_EDITOR
    bool ImplementsGetWorld() const override;
#endif

    /**
     * Bind a delegate to the callback when an item is done.
     * @param Delegate The callback for when the item has resolved.
     * @return The handle to the delegate in question.
     */
    FDelegateHandle BindToEffectComplete(FOnItemEffectComplete::FDelegate &&Delegate);

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

namespace Pokemon::Items {
    template <typename T, typename... A>
    concept FieldItem = std::is_base_of_v<UFieldItemEffect, T>
        && requires(T& Effect, const FItem& Item, int32 Quantity, A&&... Args) {
              Effect.Use(Item, Quantity, Forward<A>(Args)...);
        };
}