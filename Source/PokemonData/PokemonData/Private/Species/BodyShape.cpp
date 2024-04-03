// "Unreal Pokémon" created by Retro & Chill.
#include "Species/BodyShape.h"

#include "DataManager.h"

TArray<FName> UBodyShapeHelper::GetBodyShapeNames() {
	return FDataManager::GetInstance().GetDataTable<FBodyShape>().GetTableRowNames();
}
