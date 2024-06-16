// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TraitHolder.generated.h"

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType)
class UTraitHolder : public UInterface {
    GENERATED_BODY()
};

/**
 * Interface to describe an object that holds some type of traits
 */
class POKEMONBATTLE_API ITraitHolder {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    /**
     * Check if the trait holder possesses the given tag
     * @param Tag The tag to check if the trait holder has
     * @return Does the trait holder have the given tag?
     */
    virtual bool HasTag(FName Tag) const = 0;
    
};