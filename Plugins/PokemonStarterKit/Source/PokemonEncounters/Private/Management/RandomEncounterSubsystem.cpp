// "Unreal Pokémon" created by Retro & Chill.

#include "Management/RandomEncounterSubsystem.h"
#include "AbilitySystemComponent.h"
#include "Algo/AnyOf.h"
#include "Algo/ForEach.h"
#include "EncounterData/MapEncounterData.h"
#include "GameFramework/Character.h"
#include "Management/RandomEncounterAttributeSet.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"
#include "Trainers/Trainer.h"
#include "Utilities/TrainerHelpers.h"

static float GetMultiplier(const UAbilitySystemComponent *AbilitySystemComponent,
                           const FGameplayAttribute &GameplayAttribute) {
    bool bFound;
    auto Multiplier = AbilitySystemComponent->GetGameplayAttributeValue(GameplayAttribute, bFound);
    return bFound ? Multiplier : 1.f;
}

void URandomEncounterSubsystem::SetEncounterData(AMapEncounterData *Data) {
    EncounterData = Data;
}

bool URandomEncounterSubsystem::HasEncountersForType(const FGameplayTag &EncounterType) const {
    if (!IsValid(EncounterData)) {
        return false;
    }

    return Algo::AnyOf(EncounterData->GetEncounters(), [&EncounterType](const TPair<FGameplayTag, FEncounterData>& Pair) {
        return Pair.Key.MatchesTag(EncounterType);
    });
}

bool URandomEncounterSubsystem::HasEncountersForTypeExact(const FGameplayTag &EncounterType) const {
    return IsValid(EncounterData) && EncounterData->GetEncounters().Contains(EncounterType);
}

bool URandomEncounterSubsystem::RequestEncounterForType(const FGameplayTag &EncounterType,
                                                        FRetrievedEncounter &Encounter, int32 ChanceRolls) const {
    if (!IsValid(EncounterData)) {
        return false;
    }

    auto &Encounters = EncounterData->GetEncounters();
    auto Data = Encounters.Find(EncounterType);
    if (Data == nullptr) {
        return false;
    }

    auto EncounterList = Data->Encounters;
    EncounterList.Sort([](const FEncounterEntry &A, const FEncounterEntry &B) { return A.Chance > B.Chance; });
    int32 ChanceTotal = 0;
    Algo::ForEach(EncounterList, [&ChanceTotal](const FEncounterEntry &Entry) { ChanceTotal += Entry.Chance; });

    int32 RandomNumber = 0;
    for (int32 i = 0; i < ChanceRolls; i++) {
        if (int32 Value = FMath::Rand() % ChanceTotal; Value > RandomNumber) {
            RandomNumber = Value;
        }
    }

    const FEncounterEntry *EncounterEntry = nullptr;
    for (const auto &Entry : EncounterList) {
        RandomNumber -= Entry.Chance;
        if (RandomNumber >= 0) {
            continue;
        }

        EncounterEntry = &Entry;
    }

    check(EncounterEntry != nullptr)
    Encounter.Species = EncounterEntry->Species;
    Encounter.Level = FMath::RandRange(EncounterEntry->LevelRange.GetLowerBoundValue(),
                                       EncounterEntry->LevelRange.GetUpperBoundValue());
    return true;
}

bool URandomEncounterSubsystem::CheckEncounterTriggered(ACharacter *PlayerCharacter, const FGameplayTag &EncounterType,
                                                        bool bRepelActive, bool bTriggeredByStep) {
    if (bEncountersDisabled || !IsValid(EncounterData)) {
        return false;
    }

    auto &Encounters = EncounterData->GetEncounters();
    auto Data = Encounters.Find(EncounterType);
    if (Data == nullptr || Data->TriggerChance == 0) {
        return false;
    }

    auto EncounterChance = static_cast<float>(Data->TriggerChance);
    auto MinStepsNeeded = FMath::Clamp(8 - (EncounterChance / 10), 0, 8);

    auto PlayerAbilities = PlayerCharacter->GetComponentByClass<UAbilitySystemComponent>();
    check(PlayerAbilities != nullptr)
    if (bTriggeredByStep) {
        EncounterChance += static_cast<float>(ChanceAccumulator / 200);
        EncounterChance *=
            GetMultiplier(PlayerAbilities, URandomEncounterAttributeSet::GetEncounterStepModifierAttribute());
    }

    EncounterChance *=
        GetMultiplier(PlayerAbilities, URandomEncounterAttributeSet::GetEncounterChanceModifierAttribute());
    MinStepsNeeded *=
        GetMultiplier(PlayerAbilities, URandomEncounterAttributeSet::GetMinStepsNeededModifierAttribute());

    // TODO: Trigger out of battle ability effects

    // Wild encounters are much less likely to happen for the first few steps after a previous wild encounter
    if (bTriggeredByStep && static_cast<float>(StepCount) < MinStepsNeeded) {
        StepCount++;
        if (FMath::Rand() % 100 >= EncounterChance * 5 / static_cast<float>((Data->TriggerChance + ChanceAccumulator / 200))) {
            return false;
        }
    }

    if (FMath::Rand() % 100 < EncounterChance) {
        return true;
    }

    if (bTriggeredByStep) {
        if (bRepelActive) {
            ChanceAccumulator = 0;
        } else {
            ChanceAccumulator += Data->TriggerChance;
        }
    }

    return false;
}

bool URandomEncounterSubsystem::AllowEncounter(const FRetrievedEncounter &Encounter, bool bRepelActive) {
    if (bRepelActive) {
        if (auto FirstPokemon = UTrainerHelpers::GetPlayerCharacter(this)->GetPokemon(0);
            FirstPokemon != nullptr && Encounter.Level < FirstPokemon->GetStatBlock()->GetLevel()) {
            ChanceAccumulator = 0;
            return false;
        }
    }

    // TODO: Add field ability checks
    return true;
}