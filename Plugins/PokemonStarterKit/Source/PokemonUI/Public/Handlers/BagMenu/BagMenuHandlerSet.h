// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "BagMenuHandlerSet.generated.h"

class UBagMenuHandler;

/**
 * Data asset that represents a list of handlers for the bag menu.
 */
UCLASS()
class POKEMONUI_API UBagMenuHandlerSet : public UDataAsset {
    GENERATED_BODY()

  public:
    /**
     * Get the list of handlers to use
     * @return The list of handlers to use
     */
    const TArray<TObjectPtr<UBagMenuHandler>> &GetHandlers() const;

  private:
    /**
     * The list of handlers to use
     */
    UPROPERTY(EditAnywhere, Instanced, Category = "Handlers")
    TArray<TObjectPtr<UBagMenuHandler>> Handlers;
};
