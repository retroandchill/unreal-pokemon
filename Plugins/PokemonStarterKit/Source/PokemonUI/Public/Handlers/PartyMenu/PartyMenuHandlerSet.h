// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "PartyMenuHandlerSet.generated.h"

class UPartyMenuHandler;

/**
 * Data asset that represents a list of handlers for the party menu.
 */
UCLASS()
class POKEMONUI_API UPartyMenuHandlerSet : public UDataAsset {
    GENERATED_BODY()

  public:
    /**
     * Get the list of handlers to use
     * @return The list of handlers to use
     */
    const TArray<TObjectPtr<UPartyMenuHandler>> &GetHandlers() const;

  private:
    /**
     * The list of handlers to use
     */
    UPROPERTY(EditAnywhere, Instanced, Category = "Handlers")
    TArray<TObjectPtr<UPartyMenuHandler>> Handlers;
};
