// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "FieldItemEffect.h"
#include "FieldItemEffectDirect.generated.h"

struct FItem;

/**
 * Item effect for direct usage of an item.
 */
UCLASS()
class POKEMONITEMS_API UFieldItemEffectDirect : public UFieldItemEffect {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, Category = Items)
    void Use(const FItem &Item, int32 Quantity);

};
