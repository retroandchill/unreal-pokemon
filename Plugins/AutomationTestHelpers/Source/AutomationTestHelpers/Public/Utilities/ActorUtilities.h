// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "ActorUtilities.generated.h"

/**
 * Utility library for actors
 */
UCLASS()
class AUTOMATIONTESTHELPERS_API UActorUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

    /**
     *	Find all ChildActors with the specified interface
     *	@param Actor The actor to search
     *	@param Interface	Interface to find. Must be specified or result array will be empty.
     *	@param bIncludeDescendants Should the decendants be considered or just the immediate children
     *	@param OutActors	Output array of Actors of the specified interface.
     */
    UFUNCTION(BlueprintCallable, Category = "Actor",
              meta = (DeterminesOutputType = "Interface", DynamicOutputParam = "OutActors"))
    static void GetAllChildActorsWithInterface(const AActor *Actor, TSubclassOf<UInterface> Interface,
                                               bool bIncludeDescendants, TArray<AActor *> &OutActors);
};
