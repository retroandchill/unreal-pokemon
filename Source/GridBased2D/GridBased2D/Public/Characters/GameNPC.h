// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "GameCharacter.h"
#include "Interaction/Interactable.h"
#include "GameNPC.generated.h"

/**
 * Represents a basic NPC in the world.
 */
UCLASS(Blueprintable)
class GRIDBASED2D_API AGameNPC : public AGameCharacter, public IInteractable {
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
};
