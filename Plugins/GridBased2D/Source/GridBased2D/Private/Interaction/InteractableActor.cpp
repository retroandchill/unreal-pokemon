// "Unreal Pokémon" created by Retro & Chill.

#include "Interaction/InteractableActor.h"

void AInteractableActor::OnInteract_Implementation(const TScriptInterface<IGridMovable> &Character,
                                                   EInteractionType InteractionType)
{
    Interact.Broadcast(InteractionType);
}

int32 AInteractableActor::GetInteractionTypes_Implementation() const
{
    return InteractionTypes;
}
