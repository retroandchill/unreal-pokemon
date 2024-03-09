// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperTileMapComponent.h"
#include "Components/BoxComponent.h"
#include "GridBasedMap.generated.h"

UCLASS(Blueprintable, ClassGroup=(Map))
class GRIDBASED2D_API AGridBasedMap : public AActor {
	GENERATED_BODY()

public:
	/**
	 * Sets default values for this actor's properties
	 */
	AGridBasedMap();

	void PostInitProperties() override;
	void PostReinitProperties() override;
	void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
	void PostLoad() override;
	void PostEditMove(bool bFinished) override;

private:
	/**
	 * Set up the location of the map based on the configured properties
	 * @param bFinishedMoving Has moving the actor finished
	 */
	void SetUpMapLocation(bool bFinishedMoving = true);

	/**
	 * Set up the locations of the bounds relative to the tilemap
	 * @param bIsInitializing Is this being called from the constructor
	 */
	void SetBoundsPositions(bool bIsInitializing = false);

	/**
	 * The TileMap object that this map holds.
	 */
	UPROPERTY(EditAnywhere, Category = "Display")
	TObjectPtr<UPaperTileMap> TileMap;
	
	/**
	 * The tilemap to use for this asset
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Map, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPaperTileMapComponent> TileMapComponent;

	/**
	 * The bounds at the top of the map
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Map, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> TopBounds;

	/**
	 * The bounds at the bottom of the map
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Map, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BottomBounds;

	/**
	 * The bounds at the left of the map
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Map, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> LeftBounds;

	/**
	 * The bounds at the right of the map
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Map, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> RightBounds;

	/**
	 * The layer of the tilemap that is at the same level as the player
	 */
	UPROPERTY(EditAnywhere, Category = "Display", meta = (UIMin = 0, ClampMin = 0))
	int32 PlayerLevelLayer = 1;
};
