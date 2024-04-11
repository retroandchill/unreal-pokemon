// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridBasedMapBase.generated.h"
#include "MapGrid.h"

/**
 * Abstract implementation of grid based maps that contain any common functionality used by all maps
 */
UCLASS(Abstract, ClassGroup = (Map))
class GRIDBASED2D_API AGridBasedMapBase : public AActor, public IMapGrid {
    GENERATED_BODY()

  protected:
    void BeginPlay() override;

  public:
    FIntRect GetBounds() const override;
    UFUNCTION(BlueprintPure, Category = Maps)
    bool IsObjectInMap(TScriptInterface<IGridMovable> Object) const override;
    bool IsPositionInMap(const FIntVector2 &Position) const override;
    bool IsCharacterPartOfMap(const TScriptInterface<IGridMovable> &Character) const override;
    void AddCharacter(const TScriptInterface<IGridMovable> &Character) override;
    void RemoveCharacter(const TScriptInterface<IGridMovable> &Character) override;
    void OnPlayerEnter() override;

  private:
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
