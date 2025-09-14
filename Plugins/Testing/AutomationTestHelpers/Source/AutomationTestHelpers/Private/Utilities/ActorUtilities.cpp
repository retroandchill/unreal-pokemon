// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/ActorUtilities.h"

void UActorUtilities::GetAllChildActorsWithInterface(const AActor *Actor, TSubclassOf<UInterface> Interface,
                                                     bool bIncludeDescendants, TArray<AActor *> &OutActors)
{
    Actor->GetAttachedActors(OutActors, true, bIncludeDescendants);
    OutActors = OutActors.FilterByPredicate(
        [&Interface](const AActor *ChildActor) { return ChildActor->GetClass()->ImplementsInterface(Interface); });
}