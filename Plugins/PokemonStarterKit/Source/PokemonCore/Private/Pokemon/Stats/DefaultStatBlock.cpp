// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Stats/DefaultStatBlock.h"
#include "Blueprints/UtilityNodeSubsystem.h"
#include "DataManager.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/PokemonSubsystem.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Pokemon/Stats/DefaultMainBattleStatEntry.h"
#include "Pokemon/Stats/DefaultMainStatEntry.h"
#include "Pokemon/Stats/StatUtils.h"
#include "PokemonDataSettings.h"
#include "RetroLib.h"
#include "Species/Nature.h"
#include "Species/SpeciesData.h"
#include "Species/Stat.h"

using namespace StatUtils;

/**
 * Helper function to find a nature by the given ID
 * @param Nature The nature to look up
 * @return The found nature
 */
const FNature &FindNature(FName Nature) {
    const auto &DataSubsystem = FDataManager::GetInstance();
    auto &NatureTable = DataSubsystem.GetDataTable<FNature>();

    auto Ret = NatureTable.GetData(Nature);
    check(Ret != nullptr)
    return *Ret;
}

void UDefaultStatBlock::Initialize(const TScriptInterface<IPokemon> &NewOwner, const FPokemonDTO &DTO) {
    Owner = NewOwner;
    Level = DTO.Level;

    auto &GrowthRate = UPokemonSubsystem::GetInstance(this).GetGrowthRate(Owner->GetSpecies().GrowthRate);
    Exp = FMath::Max(GrowthRate.ExpForLevel(Level), DTO.Exp.Get(0));
    Nature = DTO.Nature;

    const auto &DataSubsystem = FDataManager::GetInstance();
    auto &StatTable = DataSubsystem.GetDataTable<FStat>();

    using enum EPokemonStatType;
    // clang-format off
    Stats = StatTable.GetAllRows() |
            Retro::Ranges::Views::Filter([](const FStat &Stat) {
                return Stat.Type != Battle;
            }) |
            UE::Ranges::ToMap(&FStat::ID)([this, &DTO](const FStat &Stat) {
                auto IV = DTO.IVs.Contains(Stat.ID) ? TOptional(DTO.IVs[Stat.ID]) : TOptional<int32>();
                int32 EV = DTO.EVs.Contains(Stat.ID) ? DTO.EVs[Stat.ID] : 0;
                if (Stat.Type == Main) {
                    return NewObject<UDefaultMainStatEntry>(this)->Initialize(Stat.ID, IV, EV);
                }

                return NewObject<UDefaultMainBattleStatEntry>(this)->Initialize(Stat.ID, IV, EV);
            });
    // clang-format on
}

int32 UDefaultStatBlock::GetLevel() const {
    return Level;
}

int32 UDefaultStatBlock::GetExp() const {
    return Exp;
}

int32 UDefaultStatBlock::GetExpForNextLevel() const {
    if (Level == GetMaxLevel())
        return 0;

    return UPokemonSubsystem::GetInstance(this).GetGrowthRate(Owner->GetSpecies().GrowthRate).ExpForLevel(Level + 1);
}

float UDefaultStatBlock::GetExpPercent() const {
    if (Level == GetMaxLevel()) {
        return 0.f;
    }

    auto &GrowthRate = UPokemonSubsystem::GetInstance(this).GetGrowthRate(Owner->GetSpecies().GrowthRate);
    auto ExpNeededForLevel = static_cast<float>(GrowthRate.ExpForLevel(Level));
    float TotalNeededForLevel = static_cast<float>(GrowthRate.ExpForLevel(Level + 1)) - ExpNeededForLevel;
    return (static_cast<float>(Exp) - ExpNeededForLevel) / TotalNeededForLevel;
}

FLevelUpStatChanges UDefaultStatBlock::GainExp(int32 Change, bool bShowMessages, const FLevelUpEnd &OnEnd) {
    FLevelUpStatChanges Changes;
    float ExpPercent = GetExpPercent();
    Changes.ExpPercentChange.Before = ExpPercent;
    Changes.ExpPercentChange.After = ExpPercent;
    Exp += Change;

    Changes.LevelChange.Before = Level;
    Changes.LevelChange.After = Level;
    for (auto &[ID, Stat] : Stats) {
        int32 StatValue = Stat->GetStatValue();
        Changes.StatChanges.Emplace(ID, {.Before = StatValue, .After = StatValue});
    }
    while (Exp >= GetExpForNextLevel()) {
        Level++;
    }

    if (Level > Changes.LevelChange.Before) {
        Changes.LevelChange.After = Level;
        Changes.ExpPercentChange.After = GetExpPercent();
        CalculateStats(Owner->GetSpecies().BaseStats);
        auto HPStat = GetDefault<UPokemonDataSettings>()->HPStat;
        for (auto &[ID, Stat] : Stats) {
            Changes.StatChanges[ID].After = Stat->GetStatValue();
            if (ID == HPStat) {
                int32 Diff = Changes.StatChanges[ID].Diff();
                Owner->SetCurrentHP(Owner->GetCurrentHP() + Diff);
            }
        }

        auto Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UUtilityNodeSubsystem>();
        Subsystem->ExecuteUtilityFunction<UUtility_ProcessLevelUp>(
            Owner, Changes, bShowMessages, FSimpleDelegate::CreateLambda([OnEnd] { OnEnd.ExecuteIfBound(); }));
    }

    return Changes;
}

const FNature &UDefaultStatBlock::GetNature() const {
    if (Nature.IsSet())
        return FindNature(Nature.GetValue());

    auto &DataTable = FDataManager::GetInstance().GetDataTable<FNature>();
    auto NatureRows = DataTable.GetTableRowNames();
    auto Index = static_cast<int32>(Owner->GetPersonalityValue() % NatureRows.Num());
    return *DataTable.GetData(NatureRows[Index]);
}

TScriptInterface<IStatEntry> UDefaultStatBlock::GetStat(FMainStatHandle Stat) const {
    check(Stats.Contains(Stat))
    return Stats[Stat];
}

void UDefaultStatBlock::ForEachStat(const TFunctionRef<void(FName, const IStatEntry &)> &Predicate) const {
    for (const auto &[Key, Value] : Stats) {
        Predicate(Key, *Value);
    }
}

void UDefaultStatBlock::CalculateStats(const TMap<FName, int32> &BaseStats) {
    auto &NatureData = GetNature();

    for (const auto &[StatID, Stat] : Stats) {
        check(BaseStats.Contains(StatID))
        Stat->RefreshValue(Level, BaseStats[StatID], NatureData);
    }
}