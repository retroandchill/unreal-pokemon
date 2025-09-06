// "Unreal Pokémon" created by Retro & Chill.

#include "Interop/GameDataRepositoryExporter.h"
#include "GameDataRepository.h"

const FScriptArray *UGameDataRepositoryExporter::GetEntries(const UGameDataRepository *Repository)
{
    return Repository->DataArray;
}

const uint8 *UGameDataRepositoryExporter::GetEntryByID(const UGameDataRepository *Repository, const FName ID)
{
    return Repository->GetEntryByID(ID);
}

const uint8 *UGameDataRepositoryExporter::GetEntryByIndex(const UGameDataRepository *Repository, const int32 Index)
{
    return Repository->GetEntryAtIndex(Index);
}