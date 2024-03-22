// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TileReplacerComponent.generated.h"

class ATileReplacementActor;
class UPaperTileMapComponent;

/**
 * Actor component used for replacing specific tiles in a tilemap with an animated actor instead. 
 */
UCLASS(ClassGroup=("Animated Tiles"), meta=(BlueprintSpawnableComponent))
class TILEREPLACER_API UTileReplacerComponent : public UActorComponent {
	GENERATED_BODY()

public:
	/**
	 * Sets default values for this component's properties.
	 */
	UTileReplacerComponent();

	/**
	 * Replace all specified tiles in the tilemap with the animated material.
	 * <p></p>
	 * <p><b>NOTE:</b> This is very inefficient, and should not be used during runtime. This should be called in the Editor
	 * and on cook.</p>
	 * @param TilemapComponent The tilemap component to use.
	 */
	void ReplaceTiles(UPaperTileMapComponent *TilemapComponent) const;

protected:
	void BeginPlay() override;

public:
	void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	/**
	 * The table used to look up tile replacements
	 */
	UPROPERTY(EditAnywhere, Category = "Data")
	TObjectPtr<UDataTable> TileReplacementTable;

	/**
	 * The type of the component that needs to be replaced
	 */
	UPROPERTY(EditAnywhere, Category = "Tile Replacement")
	TSubclassOf<ATileReplacementActor> TileReplacementClass;
	
};
