#include "Asserts.h"
#include "Battle/Traits/CriticalHits/SimpleCriticalHitModificationTrait.h"
#include "Misc/AutomationTest.h"
#include "Utilities/ReflectionUtils.h"
#include "UtilityClasses/Dispatchers/TestCritRateModCondition.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestCritRateModificationConditions, "Unit Tests.Battle.Traits.TestCritRateModificationConditions",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestCritRateModificationConditions::RunTest(const FString &Parameters) {
    auto CritRateTrait = NewObject<USimpleCriticalHitModificationTrait>();
    UE_CHECK_TRUE(CritRateTrait->MeetsConditions(nullptr, nullptr));

    auto &Conditions = UReflectionUtils::GetMutablePropertyValue<TArray<TObjectPtr<UCriticalHitRateModificationCondition>>>(CritRateTrait, "Conditions");
    auto Cond1 = NewObject<UTestCritRateModCondition>(CritRateTrait);
    Cond1->bState = false;
    Conditions.Emplace(Cond1);
    auto Cond2 = NewObject<UTestCritRateModCondition>(CritRateTrait);
    Cond2->bState = true;
    Conditions.Emplace(Cond2);
    UE_CHECK_FALSE(CritRateTrait->MeetsConditions(nullptr, nullptr));
    
    Cond1->bState = true;
    UE_CHECK_TRUE(CritRateTrait->MeetsConditions(nullptr, nullptr));
    
    return true;
}