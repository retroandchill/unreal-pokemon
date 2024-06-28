// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/GameplayAbilities/Effects/StatStageEffect.h"
#include "DataManager.h"
#include "Species/PokemonStatType.h"
#include "Species/Stat.h"

#if WITH_EDITOR
void UStatStageEffect::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) {
    Super::PostEditChangeProperty(PropertyChangedEvent);

    static auto& StatsTable = FDataManager::GetInstance().GetDataTable<FStat>();
    Modifiers.Reset();
    StatsTable.ForEach([this](const FStat& Stat) {
        if (Stat.Type == EPokemonStatType::Main) {
            return;
        }
        
        auto &Multiplier = Modifiers.Emplace_GetRef();
        Multiplier.Attribute = Stat.BaseAttribute;
        Multiplier.ModifierOp = EGameplayModOp::Type::Multiplicitive;
        FAttributeBasedFloat MultiplierData;
        MultiplierData.BackingAttribute.AttributeToCapture = Stat.StagesAttribute;
        MultiplierData.BackingAttribute.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
        MultiplierData.AttributeCurve = Stat.Type == EPokemonStatType::MainBattle ? MainStatMultipliers : BattleStatMultipliers;
        Multiplier.ModifierMagnitude = FGameplayEffectModifierMagnitude(MultiplierData);
        
        auto &Divisor = Modifiers.Emplace_GetRef();
        Divisor.Attribute = Stat.BaseAttribute;
        Divisor.ModifierOp = EGameplayModOp::Type::Division;
        FAttributeBasedFloat DivisorData;
        DivisorData.BackingAttribute.AttributeToCapture = Stat.StagesAttribute;
        DivisorData.BackingAttribute.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
        DivisorData.AttributeCurve = Stat.Type == EPokemonStatType::MainBattle ? MainStatDivisors : BattleStatDivisors;
        Divisor.ModifierMagnitude = FGameplayEffectModifierMagnitude(DivisorData);
    });
}
#endif