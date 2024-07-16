#include "Asserts.h"
#include "PrimaryGameLayout.h"
#include "RangeHelpers.h"
#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Components/ProgressBar.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Mocks/MockBattle.h"
#include "Mocks/MockBattler.h"
#include "Mocks/MockBattleSide.h"
#include "Nodes/PlayBattlerHPAnimation.h"
#include "Components/NumberImageWidget.h"
#include "Screens/PokemonBattleScreen.h"
#include "Utilities/PlayerUtilities.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "Species/SpeciesData.h"

using namespace testing;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestHPBarDrain, "Unit Tests.Battle.UI.TestHPBarDrain",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestHPBarDrain::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonBattleScreen>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto [Player, Pawn] = UPlayerUtilities::CreateTestPlayer(*World);
    auto Layout = UPrimaryGameLayout::GetPrimaryGameLayout(Player.Get());
    UE_ASSERT_NOT_NULL(Layout);
    auto Screen = Layout->PushWidgetToLayerStack<UPokemonBattleScreen>(RPG::Menus::PrimaryMenuLayerTag, WidgetClass);

    CREATE_MOCK(IBattle, Battle, FMockBattle, MockBattle);

    CREATE_MOCK(IBattleSide, Side1, FMockBattleSide, MockSide1);
    CREATE_MOCK(IBattleSide, Side2, FMockBattleSide, MockSide2);
    TArray<TScriptInterface<IBattleSide>, TInlineAllocator<2>> Sides = { Side1, Side2 };
    ON_CALL(MockBattle, GetSides).WillByDefault(Return(RangeHelpers::CreateRange(Sides)));

    TOptional<FStatusEffectInfo> StatusEffectInfo;
    
    CREATE_MOCK_ACTOR(World.Get(), IBattler, Battler1, FMockBattler, MockBattler1);
    auto Battler1Actor = Cast<AActor>(Battler1.GetObject());
    auto Battler1AbilityComponent = static_cast<UBattlerAbilityComponent*>(Battler1Actor->AddComponentByClass(UBattlerAbilityComponent::StaticClass(), false, FTransform(), false));
    ON_CALL(MockBattler1, GetAbilityComponent).WillByDefault(Return(Battler1AbilityComponent));
    EXPECT_CALL(MockBattler1, IsFainted).WillOnce(Return(false)).WillOnce(Return(false)).WillRepeatedly(Return(true));
    ON_CALL(MockBattler1, GetNickname).WillByDefault(Return(FText::FromStringView(TEXT("Lucario"))));
    ON_CALL(MockBattler1, GetPokemonLevel).WillByDefault(Return(50));
    EXPECT_CALL(MockBattler1, GetHPPercent).WillOnce(Return(1.f)).WillOnce(Return(0.5f));
    ON_CALL(MockBattler1, GetGender).WillByDefault(Return(EPokemonGender::Male));
    ON_CALL(MockBattler1, GetExpPercent).WillByDefault(Return(0.25f));
    ON_CALL(MockBattler1, GetStatusEffect).WillByDefault(ReturnRef(StatusEffectInfo));
    TArray Side1Battlers = { Battler1 };
    ON_CALL(MockSide1, GetBattlers).WillByDefault(ReturnRef(Side1Battlers));

    CREATE_MOCK_ACTOR(World.Get(), IBattler, Battler2, FMockBattler, MockBattler2);
    auto Battler2Actor = Cast<AActor>(Battler1.GetObject());
    auto Battler2AbilityComponent = static_cast<UBattlerAbilityComponent*>(Battler2Actor->AddComponentByClass(UBattlerAbilityComponent::StaticClass(), false, FTransform(), false));
    ON_CALL(MockBattler2, GetAbilityComponent).WillByDefault(Return(Battler2AbilityComponent));
    EXPECT_CALL(MockBattler2, IsFainted).WillOnce(Return(false)).WillOnce(Return(false)).WillRepeatedly(Return(true));
    ON_CALL(MockBattler2, GetNickname).WillByDefault(Return(FText::FromStringView(TEXT("Lucario"))));
    ON_CALL(MockBattler2, GetPokemonLevel).WillByDefault(Return(50));
    EXPECT_CALL(MockBattler2, GetHPPercent).WillOnce(Return(1.f)).WillOnce(Return(0.5f));
    ON_CALL(MockBattler2, GetGender).WillByDefault(Return(EPokemonGender::Male));
    ON_CALL(MockBattler2, GetExpPercent).WillByDefault(Return(0.25f));
    ON_CALL(MockBattler2, GetStatusEffect).WillByDefault(ReturnRef(StatusEffectInfo));
    TArray Side2Battlers = { Battler2 };
    ON_CALL(MockSide2, GetBattlers).WillByDefault(ReturnRef(Side2Battlers));

    Battler1Actor->DispatchBeginPlay();
    Battler2Actor->DispatchBeginPlay();
    Battler1AbilityComponent->GetCoreAttributes()->InitHP(100);
    Battler1AbilityComponent->GetCoreAttributes()->InitMaxHP(100);
    Battler2AbilityComponent->GetCoreAttributes()->InitHP(100);
    Battler2AbilityComponent->GetCoreAttributes()->InitMaxHP(100);
    Screen->SetBattle(Battle);

    Battler1AbilityComponent->GetCoreAttributes()->InitHP(50);
    auto Node = UPlayBattlerHPAnimation::PlayBattlerHPAnimation(World.Get(), Battler1);
    Node->Activate();

    auto &Panels = UReflectionUtils::GetPropertyValue<TArray<TObjectPtr<UPokemonBattlePanel>>>(Screen, "Panels");
    UPokemonBattlePanel *Panel = Panels[0];

    FIND_CHILD_WIDGET(Panel, UProgressBar, HPBar);
    UE_ASSERT_NOT_NULL(HPBar);
    FIND_CHILD_WIDGET(Panel, UNumberImageWidget, CurrentHP);
    UE_ASSERT_NOT_NULL(CurrentHP);

    for (int32 i = 0; i < 10; i++) {
        FTickableGameObject::TickObjects(nullptr, LEVELTICK_All, false, 0.1f);
    }

    UE_CHECK_EQUAL(0.5f, HPBar->GetPercent());
    UE_CHECK_EQUAL(50, CurrentHP->GetNumber());
    
    return true;
}