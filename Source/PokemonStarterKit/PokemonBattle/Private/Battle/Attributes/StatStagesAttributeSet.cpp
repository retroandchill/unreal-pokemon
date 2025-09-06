// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Attributes/StatStagesAttributeSet.h"
#include "Battle/Settings/PokemonBattleSettings.h"

void UStatStagesAttributeSet::PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue)
{
    if (Attribute == GetSameTypeAttackBonusAttribute() || Attribute == GetCriticalHitStagesAttribute() ||
        Attribute == GetStatStageMultiplierAttribute())
    {
        return;
    }

    static auto &StageInfo = GetDefault<UPokemonBattleSettings>()->StatStages;
    float StageLimit = static_cast<float>(StageInfo.Num());
    NewValue = FMath::Floor(FMath::Clamp(NewValue, -StageLimit, StageLimit));
}