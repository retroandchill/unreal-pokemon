#include "Asserts.h"
#include "Battle/Abilities/AbilityBattleEffect.h"
#include "Battle/Traits/CriticalHits/CriticalHitRateModificationTrait.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestBattleArmor, "Unit Tests.Battle.Abilities.TestBattleArmor",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestBattleArmor::RunTest(const FString &Parameters) {
    FManagedBattleAbility Ability(TEXT("BATTLEARMOR"));

    auto& Traits = Ability->GetCriticalHitRateModifiers().Target;
    UE_ASSERT_FALSE(Traits.IsEmpty());
    int32 Stages = 1;
    auto Override = ECriticalOverride::Always;
    Traits[0]->Apply(Stages, Override, nullptr, nullptr);
    UE_CHECK_EQUAL(1, Stages);
    UE_CHECK_EQUAL(ECriticalOverride::Never, Override);
    
    return true;
}