// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BattlerAbilityComponent.generated.h"


class UStatStagesAttributeSet;
class UPokemonCoreAttributeSet;

UCLASS(ClassGroup=("Gameplay Abilities"), meta=(BlueprintSpawnableComponent))
class POKEMONBATTLE_API UBattlerAbilityComponent : public UAbilitySystemComponent {
    GENERATED_BODY()

protected:
    void BeginPlay() override;

public:
    UPokemonCoreAttributeSet *GetCoreAttributes() const;

private:
    UPROPERTY()
    TObjectPtr<UPokemonCoreAttributeSet> CoreAttributes;

    UPROPERTY()
    TObjectPtr<UStatStagesAttributeSet> StatStagesAttributeSet;

    
};