// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Handlers/MenuHandler.h"
#include "UObject/Object.h"

#include "BagMenuHandler.generated.h"

class IInventoryScreen;
struct FItem;
/**
 * Abstract declaration for the bag screen in the party menu
 */
UCLASS(Blueprintable, Abstract, EditInlineNew)
class POKEMONUI_API UBagMenuHandler : public UMenuHandler {
    GENERATED_BODY()

  public:
    /**
     * Should this handler show the underlying command to the player?
     * @param Screen The screen that this is on
     * @param Item The item being selected
     * @param Quantity How many copies are being held
     * @return Should the command be shown?
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Menu Handlers")
    bool ShouldShow(const TScriptInterface<IInventoryScreen> &Screen, const FItem &Item, int32 Quantity) const;

    /**
     * Handle the effect of the command
     * @param Screen The screen that this is on
     * @param Item The item being selected
     * @param Quantity How many copies are being held
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Menu Handlers")
    void Handle(const TScriptInterface<IInventoryScreen> &Screen, const FItem &Item, int32 Quantity);
};
