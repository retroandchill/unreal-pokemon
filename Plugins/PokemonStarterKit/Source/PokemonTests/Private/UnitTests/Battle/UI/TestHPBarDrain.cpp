#include "Asserts.h"
#include "Battle/ActiveSide.h"
#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Components/NumberImageWidget.h"
#include "Components/ProgressBar.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Mocks/MockBattler.h"
#include "Mocks/MockBattleSide.h"
#include "Nodes/PlayBattlerHPAnimation.h"
#include "PrimaryGameLayout.h"
#include "Screens/PokemonBattleScreen.h"
#include "Utilities/PlayerUtilities.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/BattleActors/TestActiveSide.h"
#include "UtilityClasses/BattleActors/TestPokemonBattle.h"

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

    auto Pokemon1 =
        UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("GLACEON"),
                                                                                 .Level = 75,
                                                                                 .IVs = {{"ATTACK", 31}},
                                                                                 .EVs = {{"ATTACK", 104}},
                                                                                 .Nature = FName("TIMID"),
                                                                                 .Moves = {TEXT("ICEFANG")}});
    auto Pokemon2 =
        UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("GARCHOMP"),
                                                                                 .Level = 65,
                                                                                 .IVs = {{"DEFENSE", 31}},
                                                                                 .EVs = {{"DEFENSE", 92}},
                                                                                 .Nature = FName("JOLLY")});

    auto Battle = World->SpawnActor<ATestPokemonBattle>();
    auto Side1 = World->SpawnActor<ATestActiveSide>();
    Side1->Initialize(Battle, {Pokemon1}, false);
    auto Side2 = World->SpawnActor<ATestActiveSide>();
    Side2->Initialize(Battle, {Pokemon2}, false);
    Battle->Initialize({Side1, Side2});

    auto Battler1 = Side1->GetBattlers()[0];
    auto Battler2 = Side2->GetBattlers()[0];
    auto Battler1Actor = Cast<AActor>(Battler1.GetObject());
    auto Battler2Actor = Cast<AActor>(Battler2.GetObject());
    UE_ASSERT_NOT_NULL(Battler1Actor);
    UE_ASSERT_NOT_NULL(Battler2Actor);

    Battler1Actor->DispatchBeginPlay();
    Battler2Actor->DispatchBeginPlay();

    auto Battler1AbilityComponent = Battler1->GetAbilityComponent();
    auto Battler2AbilityComponent = Battler1->GetAbilityComponent();

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