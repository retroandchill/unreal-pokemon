// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PokemonType.generated.h"

/**
 * Gameplay ability class that represents a Pokémon's type in battle.
 */
UCLASS()
class POKEMONBATTLE_API UPokemonType : public UGameplayAbility {
    GENERATED_BODY()

    
public:
    UPokemonType();

protected:
#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

private:
    /**
     * The type that this ability represents
     */
    UPROPERTY(EditDefaultsOnly, Category = TypeInformation, meta = (GetOptions = "PokemonData.TypeHelper.GetTypeNames"))
    FName SourceType;

    /**
     * The tag to apply when the owner is attacking
     */
    UPROPERTY(VisibleDefaultsOnly, Category = TypeInformation)
    FGameplayTag AttackingTag;

    /**
     * The tag to apply when the owner is defending
     */
    UPROPERTY(VisibleDefaultsOnly, Category = TypeInformation)
    FGameplayTag DefendingTag;
};
