// "Unreal Pokémon" created by Retro & Chill.


#include "EncounterData/MapEncounterData.h"
#include "DataManager.h"
#include "Field/EncounterType.h"
#include "Misc/DataValidation.h"
#include "Species/SpeciesData.h"

const TMap<FGameplayTag, FEncounterData> & AMapEncounterData::GetEncounters() const {
    return Encounters;
}

#if WITH_EDITOR
EDataValidationResult AMapEncounterData::IsDataValid(FDataValidationContext &Context) const {
    auto OriginalResult = Super::IsDataValid(Context);
    
    auto &DataManager = FDataManager::GetInstance();
    auto &SpeciesTable = DataManager.GetDataTable<FSpeciesData>();

    bool bErrorsFound = false;
    for (auto &[Type, Data] : Encounters) {
        int32 EncounterChance = 0;
        for (const auto &[Chance, Species, LevelRange] : Data.Encounters) {
            if (!SpeciesTable.IsRowNameValid(Species)) {
                auto InvalidSpeciesText = NSLOCTEXT("PokemonEncounters", "InvalidSpecies", "Invalid Species: {0}");
                Context.AddError(FText::FormatOrdered(InvalidSpeciesText, FText::FromString(Species.ToString())));
                bErrorsFound = true;
            }

            EncounterChance += Chance;

            if (!LevelRange.HasLowerBound() || !LevelRange.HasUpperBound()) {
                auto InvalidLevelRange = NSLOCTEXT("PokemonEncounters", "InvalidLevelRange",
                    "Level ranges for encounter types cannot be unbounded!");
                Context.AddError(InvalidLevelRange);
                bErrorsFound = true;
            }
        }

        if (EncounterChance != 100) {
            auto InvalidChanceSum = NSLOCTEXT("PokemonEncounters", "BadSum",
                "The chances for encounter type '{0}' do not add to 100!");
            Context.AddError(FText::FormatOrdered(InvalidChanceSum, FText::FromString(Type.ToString())));
            bErrorsFound = true;
        }
    }
    
    return bErrorsFound ? EDataValidationResult::Invalid : OriginalResult;
}
#endif