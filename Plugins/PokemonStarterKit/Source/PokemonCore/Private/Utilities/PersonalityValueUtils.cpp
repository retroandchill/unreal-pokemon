// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/PersonalityValueUtils.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Pokemon/Stats/StatBlock.h"

uint32 UPersonalityValueUtils::GeneratePersonalityValue() {
    auto Lower = static_cast<uint32>(FMath::RandRange(0, LOWER_16_BITS));
    auto Upper = static_cast<uint32>(FMath::RandRange(0, LOWER_16_BITS)) << 16;
    return Lower | Upper;
}

uint32 UPersonalityValueUtils::GeneratePersonalityValue(const FPokemonDTO &DTO) {
    return DTO.PersonalityValue.IsSet() ? DTO.PersonalityValue.GetValue() : GeneratePersonalityValue();
}

void UPersonalityValueUtils::GetBestStat(const TScriptInterface<IPokemon> &Pokemon,
    const TArray<FMainStatHandle> &StatOrder, FMainStatHandle &BestStat, int32 &BestIV) {
    auto StatBlock = Pokemon->GetStatBlock();
    check(!StatOrder.IsEmpty())
    auto StartPoint = static_cast<int32>(Pokemon->GetPersonalityValue() % StatOrder.Num());
    for (int i = 0; i < StatOrder.Num(); i++) {
        if (auto Stat = StatOrder[(i + StartPoint) % StatOrder.Num()];
            BestStat.RowID.IsNone() || StatBlock->GetStat(Stat)->GetIV() > StatBlock->GetStat(BestStat)->GetIV()) {
            BestStat = Stat;
            BestIV = StatBlock->GetStat(Stat)->GetIV();
        }
    }
}
