
#include "RetroLib/Utils/Math.h"
#include "TestAdapter.h"

TEST_CASE_NAMED(FMathTest, "Unit Tests::RetroLib::Math::Lerp", "[RetroLib][Utils]")
{

    CHECK_EQUALS("Negative Duration", Retro::LinearInterpolation(30.0, 70, -3, 20), 70.0);
    CHECK_EQUALS("Negative Input", Retro::LinearInterpolation(30.f, 70.f, 20.f, -8.f), 30.0f);
    CHECK_EQUALS("Past End", Retro::LinearInterpolation(30.0, 70, 20, 30), 70.0);
    CHECK_EQUALS("Inside interval", Retro::LinearInterpolation(30.0, 70, 20, 10), 50.0);
}