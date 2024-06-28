// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AttributeMacros.h"
#include "AttributeSet.h"
#include "PokemonCoreAttributeSet.generated.h"

/**
 * Represents the core attributes that are present on a Pokémon.
 */
UCLASS()
class POKEMONBATTLE_API UPokemonCoreAttributeSet : public UAttributeSet {
    GENERATED_BODY()

public:
    ATTRIBUTE_ACCESSORS(UPokemonCoreAttributeSet, HP)
    ATTRIBUTE_ACCESSORS(UPokemonCoreAttributeSet, MaxHP)
    ATTRIBUTE_ACCESSORS(UPokemonCoreAttributeSet, Attack)
    ATTRIBUTE_ACCESSORS(UPokemonCoreAttributeSet, Defense)
    ATTRIBUTE_ACCESSORS(UPokemonCoreAttributeSet, SpecialAttack)
    ATTRIBUTE_ACCESSORS(UPokemonCoreAttributeSet, SpecialDefense)
    ATTRIBUTE_ACCESSORS(UPokemonCoreAttributeSet, Speed)

    void PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue) override;
    void PostAttributeChange(const FGameplayAttribute &Attribute, float OldValue, float NewValue) override;

private:
    UPROPERTY()
    FGameplayAttributeData HP;
    
    UPROPERTY()
    FGameplayAttributeData MaxHP;
    
    UPROPERTY()
    FGameplayAttributeData Attack;
    
    UPROPERTY()
    FGameplayAttributeData Defense;

    UPROPERTY()
    FGameplayAttributeData SpecialAttack;

    UPROPERTY()
    FGameplayAttributeData SpecialDefense;

    UPROPERTY()
    FGameplayAttributeData Speed;

    UPROPERTY()
    FGameplayAttributeData Accuracy;

    UPROPERTY()
    FGameplayAttributeData Evasion;

};
