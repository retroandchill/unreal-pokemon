// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"

#include "InteractableActor.generated.h"

/**
 * Basic actor class for interactivity
 */
UCLASS(Abstract, Blueprintable)
class GRIDBASED2D_API AInteractableActor : public AActor, public IInteractable
{
    GENERATED_BODY()

  public:
    void OnInteract_Implementation(const TScriptInterface<IGridMovable> &Character,
                                   EInteractionType InteractionType) override;
    int32 GetInteractionTypes_Implementation() const override;

  private:
    /**
     * Called when the player interacts with this object
     */
    UPROPERTY(BlueprintAssignable, Category = Interaction)
    FOnPlayerInteraction Interact;

    /**
     * The types of interaction that are allowed for this actor
     */
    UPROPERTY(EditAnywhere, Category = Interaction, meta = (Bitmask, BitmaskEnum = EInteractionType))
    uint8 InteractionTypes;
};
