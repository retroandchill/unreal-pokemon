// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "TileReplacementActor.generated.h"


/**
 * Actor component that serves as a replacement for tiles, is created and then has a material set on it, which is bound
 * to an actor which contains the given tile.
 */
UCLASS(Abstract, Blueprintable)
class TILEREPLACER_API ATileReplacementActor : public AActor {
	GENERATED_BODY()

public:

	/**
	 * Set the replacement material for this component
	 * @param Material The material to set for the component
	 * @param bCollisionEnabled Is there collision on the given tile?
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Tile Replacement")
	void SetReplacementMaterial(UMaterialInterface* Material, bool bCollisionEnabled);

};
