// "Unreal Pokémon" created by Retro & Chill.
#include "Species/SpeciesData.h"
#include "DataManager.h"
#include "Species/GenderRatio.h"

const FGenderRatio &FSpeciesData::GetGenderRatio() const
{
    auto Ret = FDataManager::GetInstance().GetDataTable<FGenderRatio>().GetData(GenderRatio);
    check(Ret != nullptr)
    return *Ret;
}

TArray<FName> USpeciesHelper::GetSpeciesNames()
{
    return FDataManager::GetInstance().GetDataTable<FSpeciesData>().GetTableRowNames();
}

FName USpeciesHelper::ConvertSpeciesHandleToName(const FSpeciesHandle &Struct)
{
    return Struct;
}

FSpeciesHandle USpeciesHelper::ConvertNameToSpeciesHandle(FName Name)
{
    return Name;
}
