// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperTileMapComponent.h"
#include "Characters/GameCharacter.h"
#include "Components/BoxComponent.h"
#include "GridBasedMap.generated.h"

class IGridMovable;
class AGameCharacter;
class UTileReplacerComponent;
class IWithinMap;

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

	/**
	 * Get the bounds of the map in grid units.
	 * @return The bounds of the map.
	 */
	FIntRect GetBounds() const;

	/**
	 * Check if the the current object is inside the map in question.
	 * @param Object The object to check the position of
	 * @return Is the object inside this bound of this map?
	 */
	UFUNCTION(BlueprintPure, Category = Maps)
	bool IsObjectInMap(TScriptInterface<IGridMovable> Object) const;

	/**
	 * Check if the given position is inside the map in question
	 * @param Position The position to check
	 * @return Is the position inside the map
	 */
	bool IsPositionInMap(const FIntVector2 &Position) const;

	/**
	 * Check if the given character is part of the map according to its list of contained characters.
	 * @param Character The characters to check?
	 * @return Does this map consider this character a part of itself?
	 */
	bool IsCharacterPartOfMap(const TScriptInterface<IGridMovable>& Character) const;

	/**
	 * Add a character to this map
	 * @param Character The character to add
	 */
	void AddCharacter(const TScriptInterface<IGridMovable>& Character);

	/**
	 * Remove a character from this map
	 * @param Character The character to remove
	 */
	void RemoveCharacter(const TScriptInterface<IGridMovable>& Character);

	/**
	 * Called when the player enters the map
	 */
	void OnPlayerEnter();

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
