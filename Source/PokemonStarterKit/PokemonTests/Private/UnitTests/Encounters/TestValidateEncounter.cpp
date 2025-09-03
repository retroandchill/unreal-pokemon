#include "Asserts.h"
#include "EncounterData/MapEncounterData.h"
#include "Misc/AutomationTest.h"
#include "Misc/DataValidation.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestValidateEncounter, "Unit Tests.Encounters.TestValidateEncounter",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestValidateEncounter::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto EncounterData = World->SpawnActor<AMapEncounterData>();

    auto &Encounters =
        UReflectionUtils::GetMutablePropertyValue<TMap<FName, FEncounterData>>(EncounterData, "Encounters");
    auto Context = MakeUnique<FDataValidationContext>();
    UE_CHECK_EQUAL(EDataValidationResult::Valid, EncounterData->IsDataValid(*Context));

    Context = MakeUnique<FDataValidationContext>();
    Encounters.Emplace(NAME_None);
    UE_CHECK_EQUAL(EDataValidationResult::Invalid, EncounterData->IsDataValid(*Context));

    Context = MakeUnique<FDataValidationContext>();
    Encounters.Reset();
    Encounters.Emplace("InvalidType");
    UE_CHECK_EQUAL(EDataValidationResult::Invalid, EncounterData->IsDataValid(*Context));

    Context = MakeUnique<FDataValidationContext>();
    Encounters.Reset();
    Encounters.Emplace("Land");
    UE_CHECK_EQUAL(EDataValidationResult::Invalid, EncounterData->IsDataValid(*Context));

    Context = MakeUnique<FDataValidationContext>();
    Encounters.Reset();
    auto &LandEncounter = Encounters.Emplace("Land");
    auto &EncounterRow1 = LandEncounter.Encounters.Emplace_GetRef();
    auto &EncounterRow2 = LandEncounter.Encounters.Emplace_GetRef();
    EncounterRow1.Chance = 50;
    EncounterRow2.Chance = 51;
    EncounterRow1.Species = "PATRAT";
    EncounterRow2.Species = NAME_None;
    EncounterRow1.LevelRange.SetLowerBound(FInt32Range::BoundsType::Inclusive(13));
    EncounterRow1.LevelRange.SetUpperBound(FInt32Range::BoundsType::Inclusive(15));
    EncounterRow2.LevelRange.SetLowerBound(FInt32Range::BoundsType::Open());
    EncounterRow2.LevelRange.SetUpperBound(FInt32Range::BoundsType::Inclusive(15));
    UE_CHECK_EQUAL(EDataValidationResult::Invalid, EncounterData->IsDataValid(*Context));

    Context = MakeUnique<FDataValidationContext>();
    EncounterRow2.Species = "PIDOVE";
    EncounterRow2.Chance = 50;
    EncounterRow2.LevelRange.SetLowerBound(FInt32Range::BoundsType::Inclusive(13));
    UE_CHECK_EQUAL(EDataValidationResult::Valid, EncounterData->IsDataValid(*Context));

    return true;
}