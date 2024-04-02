// "Unreal Pokémon" created by Retro & Chill.
#include "Species/Habitat.h"

#include "DataManager.h"

TArray<FName> UHabitatHelper::GetHabitatNames() {
	return FDataManager::GetInstance().GetDataTable<FHabitat>().GetTableRowNames();
}
