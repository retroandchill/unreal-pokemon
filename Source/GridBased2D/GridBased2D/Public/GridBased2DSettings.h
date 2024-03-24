// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GridBased2DSettings.generated.h"

/**
 * The Settings class for the Grid-Based 2D Module
 */
UCLASS(Config=Game, DefaultConfig, meta = (DisplayName = "Grid-Based 2D Settings"))
class GRIDBASED2D_API UGridBased2DSettings : public UDeveloperSettings {
	GENERATED_BODY()

public:
	/**
	 * Get the dynamic offset of a level in the game
	 * @return The dynamic offset of a level in the game
	 */
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Level Streaming")
	FVector GetDynamicLevelOffset() const;

private:
	/**
	 * The dynamic offset of a level in the game
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetDynamicLevelOffset, Config, Category = "Level Streaming")
	FVector DynamicLevelOffset;
};
