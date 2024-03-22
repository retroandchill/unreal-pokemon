// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TileReplacerSettings.generated.h"

/**
 * Developer Settings for the Tile Replacer
 */
UCLASS(Config=Game, DefaultConfig, meta = (DisplayName = "Tile Replacer Settings"))
class TILEREPLACER_API UTileReplacerSettings : public UDeveloperSettings {
	GENERATED_BODY()

public:
	/**
	 * Get the table used to replace the tiles
	 * @return The table used to replace the tiles
	 */
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Data)
	const FSoftObjectPath& GetTileReplacementsTable() const;

private:
	/**
	 * The table used to replace the tiles
	 */
	UPROPERTY(EditAnywhere, Config, BlueprintGetter=GetTileReplacementsTable, Category = "Data", meta = (AllowedClasses = "DataTable"))
	FSoftObjectPath TileReplacementsTable;
};
