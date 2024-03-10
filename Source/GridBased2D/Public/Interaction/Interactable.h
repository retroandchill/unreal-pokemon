// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

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
	 */
	UFUNCTION(BlueprintNativeEvent, Category = Interaction)
	void OnInteract(AGameCharacter* Character);
};

/**
 * Callback delegate for when the player tries to interact with this component.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerInteraction);
