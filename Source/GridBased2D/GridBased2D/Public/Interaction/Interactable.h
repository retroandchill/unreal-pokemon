// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "InteractionType.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

class IGridMovable;
class AGameCharacter;
// This class does not need to be modified.
UINTERFACE()
class UInteractable : public UInterface {
	GENERATED_BODY()
};

/**
 * Represents an object that is interactable within the world.
 */
class GRIDBASED2D_API IInteractable {
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Called when this object is interacted with by the player.
	 * @param Character The character that interacted with the object
	 * @param InteractionType The type of interaction with this character
	 */
	UFUNCTION(BlueprintNativeEvent, Category = Interaction)
	void OnInteract(const TScriptInterface<IGridMovable>& Character, EInteractionType InteractionType);

	/**
	 * Get the list of valid interaction types for this class
	 * @return The bitmask of valid interaction types
	 */
	virtual uint8 GetInteractionTypes() const = 0;
};

/**
 * Callback delegate for when the player tries to interact with this component.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerInteraction, EInteractionType, InteractionType);
