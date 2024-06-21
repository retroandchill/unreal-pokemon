#include "Asserts.h"
#include "Battle/Abilities/AbilityBattleEffect.h"
#include "Battle/Traits/CriticalHits/CriticalHitRateModificationTrait.h"
#include "Misc/AutomationTest.h"
#include "UtilityClasses/BattleActors/CriticalTestMove.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestSuperLuck, "Unit Tests.Battle.Abilities.TestSuperLuck",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestSuperLuck::RunTest(const FString &Parameters) {
    FManagedBattleAbility Ability(TEXT("SUPERLUCK"));

    auto& Traits = Ability->GetCriticalHitRateModifiers().User;
    UE_ASSERT_FALSE(Traits.IsEmpty());
    int32 Stages = 1;
    auto Override = ECriticalOverride::Normal;
    Traits[0]->Apply(Stages, Override, nullptr, nullptr);
    UE_CHECK_EQUAL(2, Stages);
    UE_CHECK_EQUAL(ECriticalOverride::Normal, Override);
    return true;
}