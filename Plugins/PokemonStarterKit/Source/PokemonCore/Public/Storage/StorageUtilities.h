// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StorageUtilities.generated.h"

class IPokemon;
class IStorageBox;
/**
 * The result of a box deposit
 */
UENUM()
enum class EDepositResult : uint8 {
    /**
     * The Pokémon was successfully deposited
     */
    Deposited,

    /**
     * The box was full so the Pokémon could not be deposited
     */
    BoxFull
};

UENUM()
enum class EWithdrawResult : uint8 {
    /**
     * A Pokémon was successfully withdrawn
     */
    PokemonPresent,

    /**
     * The withdrawn slot was empty
     */
    EmptySlot
};

/**
 * Utility library for the Pokémon storage system.
 */
UCLASS()
class POKEMONCORE_API UStorageUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Storage|Pokémon")
    static EDepositResult DepositToBox(const TScriptInterface<IStorageBox>& Box, const TScriptInterface<IPokemon>& Pokemon, int32& DepositIndex);

    UFUNCTION(BlueprintCallable, Category = "Storage|Pokémon")
    static EWithdrawResult WithdrawFromBox(const TScriptInterface<IStorageBox>& Box, int32 BoxIndex, TScriptInterface<IPokemon>& WithdrawnPokemon);

    UFUNCTION(BlueprintCallable, Category = "Storage|Pokémon")
    static EWithdrawResult SwapWithPokemon(const TScriptInterface<IStorageBox>& Box, int32 BoxIndex, const TScriptInterface<IPokemon>& Pokemon, TScriptInterface<IPokemon>& WithdrawnPokemon);

};
