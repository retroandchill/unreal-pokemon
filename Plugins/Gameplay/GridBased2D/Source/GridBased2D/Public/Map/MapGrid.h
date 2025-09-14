// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/GridMovable.h"
#include "UObject/Interface.h"

#include "MapGrid.generated.h"

enum class EFacingDirection : uint8;
class IGridMovable;

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UMapGrid : public UInterface
{
    GENERATED_BODY()
};

/**
 * Interface used to define a basic map grid that is used by characters to determine movement.
 */
class GRIDBASED2D_API IMapGrid
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    /**
     * Get the bounds of the map in grid units.
     * @return The bounds of the map.
     */
    virtual FIntRect GetBounds() const = 0;

    /**
     * Check if the the current object is inside the map in question.
     * @param Object The object to check the position of
     * @return Is the object inside this bound of this map?
     */
    UFUNCTION(BlueprintCallable, Category = Maps)
    virtual bool IsObjectInMap(TScriptInterface<IGridMovable> Object) const = 0;

    /**
     * Check if the given position is inside the map in question
     * @param Position The position to check
     * @return Is the position inside the map
     */
    virtual bool IsPositionInMap(const FIntVector2 &Position) const = 0;

    /**
     * Check if the given character is part of the map according to its list of contained characters.
     * @param Character The characters to check?
     * @return Does this map consider this character a part of itself?
     */
    virtual bool IsCharacterPartOfMap(const TScriptInterface<IGridMovable> &Character) const = 0;

    /**
     * Add a character to this map
     * @param Character The character to add
     */
    virtual void AddCharacter(const TScriptInterface<IGridMovable> &Character) = 0;

    /**
     * Remove a character from this map
     * @param Character The character to remove
     */
    virtual void RemoveCharacter(const TScriptInterface<IGridMovable> &Character) = 0;

    /**
     * Called when the player enters the map
     */
    virtual void OnPlayerEnter() = 0;
};
