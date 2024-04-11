// "Unreal Pokémon" created by Retro & Chill.
#include "Species/GenderRatio.h"

#include "DataManager.h"

TArray<FName> UGenderRatioHelper::GetGenderRatioNames() {
    return FDataManager::GetInstance().GetDataTable<FGenderRatio>().GetTableRowNames();
}
