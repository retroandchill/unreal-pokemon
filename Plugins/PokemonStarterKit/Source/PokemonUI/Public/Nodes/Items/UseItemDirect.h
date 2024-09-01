// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GiveItemToPokemon.h"
#include "UseItem.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "UseItemDirect.generated.h"

class UBagScreen;
/**
 * 
 */
UCLASS(meta = (HideThen))
class POKEMONUI_API UUseItemDirect : public UUseItem {
    GENERATED_BODY()

public:
    /**
     * Use the given item directly on the field.
     * @param Screen The screen calling the item effect
     * @param Item The item being used
     * @param Quantity The current quantity held
     * @return The created node
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Items, meta = (DefaultToSelf = Screen))
    static UUseItemDirect *UseItemDirect(UBagScreen *Screen, FName Item, int32 Quantity);

    void Activate() override;
};
