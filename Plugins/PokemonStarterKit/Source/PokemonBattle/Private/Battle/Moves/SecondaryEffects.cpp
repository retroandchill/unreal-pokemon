// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/SecondaryEffects.h"
#include "Battle/Battlers/Battler.h"
#include "Settings/BaseSettings.h"

void FAdditionalMoveEffects::AlterStatStage(const IBattler& Battler, FName Stat, int32 StatChange) {
    static const int32 MaxStages = Pokemon::FBaseSettings::Get().GetStatStages().Num();
    auto &Change = StatStageChanges.FindOrAdd(Stat);
    int32 CurrentStages = Battler.GetStatStage(Stat);
    int32 MinChange = -MaxStages - CurrentStages;
    int32 MaxChange = MaxStages - CurrentStages;
    Change = FMath::Clamp(Change + StatChange, MinChange, MaxChange);
}