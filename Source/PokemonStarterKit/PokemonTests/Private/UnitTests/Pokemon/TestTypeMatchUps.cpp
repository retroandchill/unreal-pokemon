#include "Asserts.h"
#include "Battle/Type.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestTypeMatchUps, "Unit Tests.Pokemon.TestTypeMatchUps",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestTypeMatchUps::RunTest(const FString &Parameters)
{
    using namespace Pokemon::TypeEffectiveness;
    UE_CHECK_EQUAL(SuperEffectiveMultiplier, UTypeHelper::GetTypeEffectiveness("FIGHTING", "NORMAL"));
    UE_CHECK_EQUAL(NormalEffectiveMultiplier, UTypeHelper::GetTypeEffectiveness("ICE", "NORMAL"));
    UE_CHECK_EQUAL(NotVeryEffectiveMultiplier, UTypeHelper::GetTypeEffectiveness("ICE", "WATER"));
    UE_CHECK_EQUAL(IneffectiveMultiplier, UTypeHelper::GetTypeEffectiveness("FIGHTING", "GHOST"));

    UE_CHECK_TRUE(IsIneffective(0.f));
    UE_CHECK_FALSE(IsIneffective(0.5f));
    UE_CHECK_FALSE(IsIneffective(1.f));
    UE_CHECK_FALSE(IsIneffective(2.f));

    UE_CHECK_FALSE(IsNotVeryEffective(0.f));
    UE_CHECK_TRUE(IsNotVeryEffective(0.5f));
    UE_CHECK_FALSE(IsNotVeryEffective(1.f));
    UE_CHECK_FALSE(IsNotVeryEffective(2.f));

    UE_CHECK_TRUE(IsResistant(0.f));
    UE_CHECK_TRUE(IsResistant(0.5f));
    UE_CHECK_FALSE(IsResistant(1.f));
    UE_CHECK_FALSE(IsResistant(2.f));

    UE_CHECK_FALSE(IsNormal(0.f));
    UE_CHECK_FALSE(IsNormal(0.5f));
    UE_CHECK_TRUE(IsNormal(1.f));
    UE_CHECK_FALSE(IsNormal(2.f));

    UE_CHECK_FALSE(IsSuperEffective(0.f));
    UE_CHECK_FALSE(IsSuperEffective(0.5f));
    UE_CHECK_FALSE(IsSuperEffective(1.f));
    UE_CHECK_TRUE(IsSuperEffective(2.f));

    UE_CHECK_EQUAL(EDamageEffectiveness::NoEffect, GetEffectivenessFromMultiplier(0.f));
    UE_CHECK_EQUAL(EDamageEffectiveness::Normal, GetEffectivenessFromMultiplier(1.f));
    UE_CHECK_EQUAL(EDamageEffectiveness::NotVeryEffective, GetEffectivenessFromMultiplier(0.5f));
    UE_CHECK_EQUAL(EDamageEffectiveness::SuperEffective, GetEffectivenessFromMultiplier(2.f));

    return true;
}