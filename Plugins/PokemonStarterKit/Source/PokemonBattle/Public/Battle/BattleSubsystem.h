// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BattleSubsystem.generated.h"

class UBattleAnimationGetter;
/**
 * 
 */
UCLASS()
class POKEMONBATTLE_API UBattleSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

public:
    void Initialize(FSubsystemCollectionBase &Collection) override;

    UBattleAnimationGetter& GetAnimationGetter() const {
        check(AnimationGetter != nullptr);
        return *AnimationGetter;
    }

    
    
private:
    UPROPERTY()
    TObjectPtr<UBattleAnimationGetter> AnimationGetter;

};

