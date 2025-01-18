// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "UE5Coro.h"

#include "ApplyMessageGameplayEffectComponent.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "Message on Apply")
class POKEMONBATTLE_API UApplyMessageGameplayEffectComponent : public UGameplayEffectComponent {
    GENERATED_BODY()

public:
    UE5Coro::TCoroutine<> DisplayApplyMessage(UE5Coro::TLatentContext<const UObject> Context, const FText& TargetName) const;

private:
    UPROPERTY(EditAnywhere)
    FText MessageFormat;

};
