// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Windows/SelectableWidget.h"
#include "ItemSelectionWindow.generated.h"

class UItemOption;

/**
 * Window for selecting an item from a particular pocket.
 */
UCLASS(Abstract)
class POKEMONUI_API UItemSelectionWindow : public USelectableWidget {
    GENERATED_BODY()

public:
    /**
     * Slot an item entry into the window
     * @param Option The option that represents an item in the player's inventory
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Display)
    void SlotItem(UItemOption* Option);

private:
    /**
     * Class that is used to spawn options into the window
     */
    UPROPERTY(EditAnywhere, Category = "Display")
    TSubclassOf<UItemOption> ItemEntryClass;

    /**
     * The list of option widgets in the window.
     */
    UPROPERTY()
    TArray<TObjectPtr<UItemOption>> Options;

};
