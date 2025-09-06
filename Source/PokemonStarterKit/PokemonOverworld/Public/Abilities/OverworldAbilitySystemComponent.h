// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "OverworldAbilitySystemComponent.generated.h"

class URandomEncounterAttributeSet;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class POKEMONOVERWORLD_API UOverworldAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()

  public:
    void BeginPlay() override;

  private:
    UPROPERTY()
    TObjectPtr<URandomEncounterAttributeSet> EncounterAttributes;
};