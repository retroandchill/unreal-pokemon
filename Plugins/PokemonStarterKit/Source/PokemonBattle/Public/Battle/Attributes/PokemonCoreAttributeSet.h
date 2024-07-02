// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AttributeMacros.h"
#include "AttributeSet.h"
#include "PokemonCoreAttributeSet.generated.h"

class IPokemon;
/**
 * Represents the core attributes that are present on a Pokémon.
 */
UCLASS()
class POKEMONBATTLE_API UPokemonCoreAttributeSet : public UAttributeSet {
    GENERATED_BODY()

public:
    ATTRIBUTE_ACCESSORS_CUSTOM_INITTER(UPokemonCoreAttributeSet, HP);
    ATTRIBUTE_ACCESSORS(UPokemonCoreAttributeSet, MaxHP)
    ATTRIBUTE_ACCESSORS(UPokemonCoreAttributeSet, Attack)
    ATTRIBUTE_ACCESSORS(UPokemonCoreAttributeSet, Defense)
    ATTRIBUTE_ACCESSORS(UPokemonCoreAttributeSet, SpecialAttack)
    ATTRIBUTE_ACCESSORS(UPokemonCoreAttributeSet, SpecialDefense)
    ATTRIBUTE_ACCESSORS(UPokemonCoreAttributeSet, Speed)
    ATTRIBUTE_ACCESSORS(UPokemonCoreAttributeSet, Accuracy)
    ATTRIBUTE_ACCESSORS(UPokemonCoreAttributeSet, Evasion)
    ATTRIBUTE_ACCESSORS(UPokemonCoreAttributeSet, MoveCost)

    void PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue) override;
    void PostAttributeChange(const FGameplayAttribute &Attribute, float OldValue, float NewValue) override;

private:
    /**
     * Called to update the Pokémon's tags based on its current HP
     */
    void UpdateHPTags() const;

    /**
     * The current HP of the battler
     */
    UPROPERTY()
    FGameplayAttributeData HP;

    /**
     * The maximum possible HP of the battler
     */
    UPROPERTY()
    FGameplayAttributeData MaxHP;

    /**
     * The physical attack of the battler
     */
    UPROPERTY()
    FGameplayAttributeData Attack;

    /**
     * The physical attack of the defense
     */
    UPROPERTY()
    FGameplayAttributeData Defense;

    /**
     * The special attack of the battler
     */
    UPROPERTY()
    FGameplayAttributeData SpecialAttack;

    /**
     * The special defense of the battler
     */
    UPROPERTY()
    FGameplayAttributeData SpecialDefense;

    /**
     * The speed of the battler
     */
    UPROPERTY()
    FGameplayAttributeData Speed;

    /**
     * The accuracy of the battler
     */
    UPROPERTY()
    FGameplayAttributeData Accuracy = 100.f;

    /**
     * The evasion rate of the battler
     */
    UPROPERTY()
    FGameplayAttributeData Evasion = 100.f;

    /**
     * The amount of PP that a move costs to use.
     */
    UPROPERTY()
    FGameplayAttributeData MoveCost = 1.f;
};
