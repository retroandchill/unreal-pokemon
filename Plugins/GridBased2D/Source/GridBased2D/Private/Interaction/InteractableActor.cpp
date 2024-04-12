// "Unreal Pokémon" created by Retro & Chill.

#include "Interaction/InteractableActor.h"

void AInteractableActor::OnInteract_Implementation(const TScriptInterface<IGridMovable> &Character,
                                                   EInteractionType InteractionType) {
    Interact.Broadcast(InteractionType);
}

uint8 AInteractableActor::GetInteractionTypes() const {
    return InteractionTypes;
}
