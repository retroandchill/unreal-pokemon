// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Moves/BaseBattleMove.h"
#include "CriticalTestMove.generated.h"

/**
 * 
 */
UCLASS()
class POKEMONTESTS_API UCriticalTestMove : public UBaseBattleMove {
    GENERATED_BODY()

public:
    ECriticalOverride CriticalOverride = ECriticalOverride::Normal;

protected:
    ECriticalOverride GetCriticalOverride_Implementation(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) const override;

};
