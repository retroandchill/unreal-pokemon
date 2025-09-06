// "Unreal Pokémon" created by Retro & Chill.
#include "Asserts.h"
#include "Characters/FacingDirection.h"
#include "GridUtils.h"
#include "Misc/AutomationTest.h"
#include <sstream>

IMPLEMENT_COMPLEX_AUTOMATION_TEST(VectorToFacingDirectionTest, "Unit Tests.Movement.VectorToFacingDirectionTest",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void VectorToFacingDirectionTest::GetTests(TArray<FString> &OutBeautifiedNames, TArray<FString> &OutTestCommands) const
{
    OutBeautifiedNames.Add("Up");
    OutTestCommands.Add("0 -1 " + FString::FromInt(static_cast<uint8>(EFacingDirection::Up)));

    OutBeautifiedNames.Add("Left");
    OutTestCommands.Add("-1 0 " + FString::FromInt(static_cast<uint8>(EFacingDirection::Left)));

    OutBeautifiedNames.Add("Right");
    OutTestCommands.Add("1 0 " + FString::FromInt(static_cast<uint8>(EFacingDirection::Right)));

    OutBeautifiedNames.Add("Down");
    OutTestCommands.Add("0 1 " + FString::FromInt(static_cast<uint8>(EFacingDirection::Down)));
}

bool VectorToFacingDirectionTest::RunTest(const FString &Parameters)
{
    std::stringstream InputStream(TCHAR_TO_UTF8(*Parameters));
    double X;
    double Y;
    int32 DirValue;
    InputStream >> X >> Y >> DirValue;

    auto ExpectedDir = static_cast<EFacingDirection>(DirValue);
    auto ActualDir = UGridUtils::VectorToFacingDirection(FVector2D(X, Y));
    UE_CHECK_TRUE(ActualDir.IsSet());
    UE_CHECK_EQUAL(ExpectedDir, ActualDir.GetValue());
    return true;
}
