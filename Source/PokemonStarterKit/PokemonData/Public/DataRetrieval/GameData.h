// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

/**
 * Basic interface to manage getting data from a data table
 */
class POKEMONDATA_API IGameData
{
  public:
    virtual ~IGameData();

    /**
     * Get the type of struct this proxy points to
     * @return The type of struct contained within the table
     */
    virtual UScriptStruct *GetStructType() const = 0;

    /**
     * Get the specified row from the data table
     * @param ID he ID to get the data from
     * @return The retrieved row from the database
     */
    virtual const FTableRowBase *GetData(FName ID) const = 0;

    /**
     * Get the specified row from the data table
     * @param ID he ID to get the data from
     * @return The retrieved row from the database
     */
    virtual const FTableRowBase &GetDataChecked(FName ID) const = 0;

    /**
     * Get the list of IDs in the table
     * @return A unique set of row names
     */
    virtual TArray<FName> GetTableRowNames() const = 0;

    /**
     * Check if the provided row name is valid or not
     * @param ID The ID to check against
     * @return If there is a row defined with the provided ID
     */
    virtual bool IsRowNameValid(FName ID) const = 0;

    /**
     * Get the underlying Data Table that holds the data
     * @return The actual Data Table asset
     */
    virtual UDataTable *GetDataTable() const = 0;
};
