// "Unreal Pokémon" created by Retro & Chill.

#include "Storage/StorageUtilities.h"
#include "Pokemon/Pokemon.h"
#include "Storage/StorageBox.h"

EDepositResult UStorageUtilities::DepositToBox(const TScriptInterface<IStorageBox> &Box,
                                               const TScriptInterface<IPokemon> &Pokemon, int32 &DepositIndex)
{
    if (auto Result = Box->DepositToBox(Pokemon); Result.IsSet())
    {
        DepositIndex = *Result;
        return EDepositResult::Deposited;
    }

    return EDepositResult::BoxFull;
}

EWithdrawResult UStorageUtilities::WithdrawFromBox(const TScriptInterface<IStorageBox> &Box, int32 BoxIndex,
                                                   TScriptInterface<IPokemon> &WithdrawnPokemon)
{
    if (auto Result = Box->WithdrawFromBox(BoxIndex); Result.IsSet())
    {
        WithdrawnPokemon = Cast<UObject>(Result.GetPtrOrNull());
        return EWithdrawResult::PokemonPresent;
    }

    return EWithdrawResult::EmptySlot;
}

EWithdrawResult UStorageUtilities::SwapWithPokemon(const TScriptInterface<IStorageBox> &Box, int32 BoxIndex,
                                                   const TScriptInterface<IPokemon> &Pokemon,
                                                   TScriptInterface<IPokemon> &WithdrawnPokemon)
{
    if (auto Result = Box->SwapWithPokemon(BoxIndex, Pokemon); Result.IsSet())
    {
        WithdrawnPokemon = Cast<UObject>(Result.GetPtrOrNull());
        return EWithdrawResult::PokemonPresent;
    }

    return EWithdrawResult::EmptySlot;
}