// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PokemonInfoWidget.h"
#include "PokemonStatDisplay.generated.h"

class UPokemonStatEntry;
struct FMainStatHandle;
class SPokemonStatGraph;
/**
 * 
 */
UCLASS()
class POKEMONUI_API UPokemonStatDisplay : public UWidget, public INamedSlotInterface {
    GENERATED_BODY()

public:
    TSharedRef<SWidget> RebuildWidget() override;

    void ReleaseSlateResources(bool bReleaseChildren) override;
    
    void GetSlotNames(TArray<FName> &SlotNames) const override;
    UWidget* GetContentForSlot(FName SlotName) const override;
    void SetContentForSlot(FName SlotName, UWidget *Content) override;

private:
    /**
     * The order of the stats that are being drawn starting from the top and going around clockwise.
     */
    UPROPERTY(EditAnywhere, Category = Content)
    TArray<FMainStatHandle> StatOrder;

    UPROPERTY()
    TMap<FName, TObjectPtr<UWidget>> StatEntries;
    
    TSharedPtr<SPokemonStatGraph> StatGraph;

};
