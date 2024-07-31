// "Unreal Pokémon" created by Retro & Chill.

#include "TestActiveSide.h"
#include "TestBattlerActor.h"
#include "Utilities/ReflectionUtils.h"

// Sets default values

ATestActiveSide::ATestActiveSide() {
    UReflectionUtils::SetPropertyValue<TSoftClassPtr<AActor>>(this, TEXT("BattlerClass"),
                                                              ATestBattlerActor::StaticClass());
}

const TArray<TScriptInterface<IBattler>> & ATestActiveSide::GetTrainerParty(const TScriptInterface<ITrainer> &Trainer) const {
    auto &Parties = UReflectionUtils::GetPropertyValue<TMap<FGuid, FTrainerParty>>(this, "TrainerParties");
    if (Trainer == nullptr || !Parties.Contains(Trainer->GetInternalId())) {
        static TArray<TScriptInterface<IBattler>> Ret;
        return Ret;
    }
    
    return Super::GetTrainerParty(Trainer);
}