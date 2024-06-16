// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "TraitHolder.h"
#include "UObject/Interface.h"
#include "IndividualTraitHolder.generated.h"

struct FIndividualDamageModifierTraits;

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType)
class UIndividualTraitHolder : public UTraitHolder {
    GENERATED_BODY()
};

/**
 * A trait holder for an individual trait.
 */
class POKEMONBATTLE_API IIndividualTraitHolder : public ITraitHolder {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    /**
     * Get the damage modifier traits
     * @return Traits that apply to modify the damage value of a move
     */
    virtual const FIndividualDamageModifierTraits& GetDamageModifiers() const = 0;

};