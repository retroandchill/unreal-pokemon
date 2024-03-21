// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

/**
 * Basic actor class for interactivity
 */
UCLASS(Abstract, Blueprintable)
class GRIDBASED2D_API AInteractableActor : public AActor, public IInteractable {
	GENERATED_BODY()

public:
	void OnInteract_Implementation(AGameCharacter* Character, EInteractionType InteractionType) override;

	UFUNCTION(BlueprintPure, Category = "Interaction")
	uint8 GetInteractionTypes() const override;

	/**
	 * Called when the player interacts with this object
	 */
	UPROPERTY(BlueprintAssignable, Category = Interaction)
	FOnPlayerInteraction Interact;

private:
	/**
	 * The types of interaction that are allowed for this actor
	 */
	UPROPERTY(EditAnywhere, Category = Interaction, meta = (Bitmask, BitmaskEnum = EInteractionType))
	uint8 InteractionTypes;
};
