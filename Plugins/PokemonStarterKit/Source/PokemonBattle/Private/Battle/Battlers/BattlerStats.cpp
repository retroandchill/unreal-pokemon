// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Battlers/BattlerStats.h"
#include "Settings/BaseSettings.h"

int32 FMainBattleStat::GetModifiedValue() const {
    static const auto &StatStages = Pokemon::FBaseSettings::Get().GetStatStages();
    check(FMath::Abs(Stages) <= StatStages.Num())
    if (Stages > 0) {
        return FMath::FloorToInt32(StatValue * StatStages[Stages - 1].PositiveStatMultiplier);
    }

    if (Stages < 0) {
        return FMath::FloorToInt32(StatValue * StatStages[-Stages - 1].NegativeStatMultiplier);
    }

    return StatValue;
}