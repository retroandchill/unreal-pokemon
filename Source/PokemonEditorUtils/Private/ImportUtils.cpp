// "Unreal Pokémon" created by Retro & Chill.
#include "ImportUtils.h"

TArray<FStat> UImportUtils::GetMainStatsFromTable(UDataTable* DataTable) {
	TArray<FStat*> References;
	DataTable->GetAllRows(TEXT("UImportUtils::GetMainStatsFromTable"), References);
	TArray<FStat> Ret;
	for (const auto Ref : References) {
		if (Ref->Type == EPokemonStatType::Battle)
			continue;
		
		Ret.Add(*Ref);
	}

	return Ret;
}
