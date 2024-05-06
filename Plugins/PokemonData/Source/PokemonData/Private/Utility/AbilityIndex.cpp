// "Unreal Pokémon" created by Retro & Chill.


#include "Utility/AbilityIndex.h"
#include "DataManager.h"
#include "PokemonDataSettings.h"
#include "Species/Ability.h"
#include "Species/SpeciesData.h"

constexpr auto NO_ABILITY_TEXT = TEXT("{0}: No Ability");
constexpr auto NO_ABILITY_HIDDEN_TEXT = TEXT("{0}: No Ability (H)");
constexpr auto REGULAR_ABILITY_TEXT = TEXT("{0}: {1}");
constexpr auto HIDDEN_ABILITY_TEXT = TEXT("{0}: {1} (H)");

FAbilityIndex::FAbilityIndex() {
#if WITH_EDITORONLY_DATA
    SetDefaultTextOptions();
#endif
}

FAbilityIndex::FAbilityIndex(int32 IndexIn) : Index(IndexIn) {
#if WITH_EDITORONLY_DATA
    SetDefaultTextOptions();
#endif
}

FAbilityIndex & FAbilityIndex::operator=(int32 NewIndex) {
    Index = NewIndex;
    return *this;
}

#if WITH_EDITORONLY_DATA
void FAbilityIndex::SetDefaultTextOptions() {
    TextOptions.Empty();
    int32 AbilityCount = GetDefault<UPokemonDataSettings>()->GetMaxDefaultAbilities();
    for (int i = 0; i < AbilityCount; i++) {
        TextOptions.Add(MakeShared<FIntTextPair>(TextOptions.Num(), FText::Format(FText::FromStringView(NO_ABILITY_TEXT), i)));
    }
    
    Index = FMath::Clamp(Index, 0, TextOptions.Num() - 1);
}

void FAbilityIndex::UpdateTextOptions(FName Species) {
    auto &DataManager = FDataManager::GetInstance();
    auto &SpeciesTable = DataManager.GetDataTable<FSpeciesData>();
    auto SpeciesData = SpeciesTable.GetData(Species);
    if (SpeciesData == nullptr) {
        SetDefaultTextOptions();
        return;
    }

    
    int32 AbilityCount = GetDefault<UPokemonDataSettings>()->GetMaxDefaultAbilities();
    auto &AbilityTable = DataManager.GetDataTable<FAbility>();
    if (SpeciesData->Abilities.IsEmpty()) {
        SetDefaultTextOptions();
    } else {
        TextOptions.Empty();
        for (int i = 0; i < AbilityCount; i++) {
            auto AbilityID = SpeciesData->Abilities[FMath::Min(i, SpeciesData->Abilities.Num() - 1)];
            if (auto AbilityData = AbilityTable.GetData(AbilityID); AbilityData != nullptr) {
                TextOptions.Add(MakeShared<FIntTextPair>(TextOptions.Num(), FText::Format(FText::FromStringView(REGULAR_ABILITY_TEXT), i, AbilityData->RealName)));
            } else {
                TextOptions.Add(MakeShared<FIntTextPair>(TextOptions.Num(), FText::Format(FText::FromStringView(NO_ABILITY_TEXT), i)));
            }
        }
    }

    for (int i = 0; i < SpeciesData->HiddenAbilities.Num(); i++) {
        auto AbilityID = SpeciesData->HiddenAbilities[FMath::Min(i, SpeciesData->Abilities.Num() - 1)];
        if (auto AbilityData = AbilityTable.GetData(AbilityID); AbilityData != nullptr) {
            TextOptions.Add(MakeShared<FIntTextPair>(TextOptions.Num(), FText::Format(FText::FromStringView(HIDDEN_ABILITY_TEXT), i + AbilityCount, AbilityData->RealName)));
        } else {
            TextOptions.Add(MakeShared<FIntTextPair>(TextOptions.Num(), FText::Format(FText::FromStringView(NO_ABILITY_HIDDEN_TEXT), i + AbilityCount)));
        }
    }
    
    Index = FMath::Clamp(Index, 0, TextOptions.Num() - 1);
}
#endif