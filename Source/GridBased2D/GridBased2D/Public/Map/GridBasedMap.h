// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "MapGrid.h"
#include "GameFramework/Actor.h"
#include "PaperTileMapComponent.h"
#include "GridBasedMap.generated.h"

class IGridMovable;
class AGameCharacter;
class UTileReplacerComponent;
class IWithinMap;

UCLASS(Blueprintable, ClassGroup=(Map))
class GRIDBASED2D_API AGridBasedMap : public AActor, public IMapGrid {
	GENERATED_BODY()

public:
	/**
	 * Sets default values for this actor's properties
	 */
	AGridBasedMap();

	void PostInitProperties() override;
	void PostReinitProperties() override;
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void PostLoad() override;
	void PostEditMove(bool bFinished) override;

	void BeginPlay() override;

#if WITH_EDITORONLY_DATA
	/**
	 * Refresh the tiles, replacing any tiles that need to be replaced
	 */
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Tiles")
	void RefreshTileData();

	/**
	 * Clear all tile replacements, restoring the original tiles to their rightful place.
	 */
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Tiles")
	void ClearTileReplacements();
#endif
	
	FIntRect GetBounds() const override;
	
	UFUNCTION(BlueprintPure, Category = Maps)
	bool IsObjectInMap(TScriptInterface<IGridMovable> Object) const  override;
	bool IsPositionInMap(const FIntVector2 &Position) const override;
	
	bool IsCharacterPartOfMap(const TScriptInterface<IGridMovable>& Character) const  override;
	void AddCharacter(const TScriptInterface<IGridMovable>& Character)  override;
	void RemoveCharacter(const TScriptInterface<IGridMovable>& Character)  override;
	
	void OnPlayerEnter()  override;

private:
	/**
	 * Set up the location of the map based on the configured properties
	 * @param bFinishedMoving Has moving the actor finished
	 */
	void SetUpMapLocation(bool bFinishedMoving = true);

	/**
	 * The tilemap to use for this asset
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Map, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPaperTileMapComponent> TileMapComponent;

#if WITH_EDITORONLY_DATA
	/**
	 * The component used to replace tiles with animated/autotiles
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Map, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTileReplacerComponent> TileReplacer;
#endif

	/**
	 * The layer of the tilemap that is at the same level as the player
	 */
	UPROPERTY(EditAnywhere, Category = "Z-Sorting", meta = (UIMin = 0, ClampMin = 0))
	int32 PlayerLevelLayer = 1;

	/**
	 * The audio played when the map starts
	 */
	UPROPERTY(EditAnywhere, Category = Audio)
	TObjectPtr<USoundBase> BackgroundMusic;

	/**
	 * The list of characters contained within this map
	 */
	UPROPERTY()
	TArray<TScriptInterface<IGridMovable>> Characters;
	
};
