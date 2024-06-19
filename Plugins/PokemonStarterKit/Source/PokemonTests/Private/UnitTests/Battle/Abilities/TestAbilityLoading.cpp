#include "Asserts.h"
#include "Battle/Abilities/AbilityBattleEffect.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Moves/BattleDamage.h"
#include "Battle/Traits/Damage/DamageModificationTrait.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Mocks/MockBattler.h"

using namespace testing;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestAbilityLoading, "Unit Tests.Battle.Abilities.TestAbilityLoading",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestAbilityLoading::RunTest(const FString &Parameters) {
    FManagedBattleAbility Ability(TEXT("TORRENT"));
    auto &Effect = *Ability;
    auto &DamageModifiers = Effect.GetDamageModifiers();

    auto [User, MockUser] = UnrealMock::CreateMock<IBattler, FMockBattler>();
    EXPECT_CALL(MockUser, GetHPPercent)
        .WillOnce(Return(0.5f))
        .WillRepeatedly(Return(0.25f));

    FMoveDamageInfo Context = {.User = User, .Type = TEXT("WATER")};
    ASSERT_FALSE(DamageModifiers.User.IsEmpty());
    CHECK_FALSE(DamageModifiers.User[0]->MeetsConditions(Context));
    CHECK_TRUE(DamageModifiers.User[0]->MeetsConditions(Context));

    FDamageMultipliers Multipliers;
    DamageModifiers.User[0]->Apply(Multipliers, Context);
    CHECK_EQUAL(1.5f, Multipliers.PowerMultiplier);

    Ability.SetID(TEXT("BLAZE"));
    auto &SecondModifiers = Ability->GetDamageModifiers();
    ASSERT_FALSE(SecondModifiers.User.IsEmpty());
    CHECK_FALSE(SecondModifiers.User[0]->MeetsConditions(Context));

    return true;
}