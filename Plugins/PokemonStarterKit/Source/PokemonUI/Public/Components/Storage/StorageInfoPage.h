// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Ranges/Optional/OptionalRef.h"
#include "StorageInfoPage.generated.h"

class IPokemon;

/**
 * Delegate invoked when the selected Pokémon in changed.
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectedPokemonChanged, TOptional<IPokemon&>);

// This class does not need to be modified.
UINTERFACE(BlueprintType, Blueprintable)
class UStorageInfoPage : public UInterface {
    GENERATED_BODY()
};

/**
 * Interface for a basic page in the storage system.
 */
class POKEMONUI_API IStorageInfoPage {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    /**
     * The Pokémon to display information for.
     * @param Pokemon The Pokémon in question.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Storage)
    void DisplayInfo(const TScriptInterface<IPokemon>& Pokemon);

};