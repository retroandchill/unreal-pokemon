#include "TestAdapter.h"

#if RETROLIB_WITH_MODULES
import std;
import RetroLib;
#else
#include "RetroLib/Optionals/Expected.h"
#include <string>
#include <vector>
#endif

TEST_CASE_NAMED(FTestExpectedOperations, "Unit Tests::RetroLib::Optionals::Expected::Operations", "[RetroLib][Optionals]") {
    SECTION("Can create an expected and check for correctness or an error") {
        Retro::TExpected<int, std::string> TestValue = 0;
        REQUIRE(TestValue.IsSet());
        CHECK(TestValue.GetValue() == 0);
        CHECK(TestValue == 0);
        
        TestValue = Retro::TUnexpected(std::string("Invalid value"));
        REQUIRE(!TestValue.IsSet());
        CHECK_THROWS_AS(TestValue.GetValue(), Retro::TBadExpectedAccess<std::string>);
        CHECK(TestValue.GetError() == "Invalid value");

        TestValue = 10;
        REQUIRE(TestValue.IsSet());
        CHECK(*TestValue == 10);
        CHECK(TestValue == 10);

        auto Copy = TestValue;
        CHECK(TestValue == Copy)

        Copy = 23;
        CHECK_FALSE(TestValue == Copy);

        Copy = Retro::TUnexpected(std::string("Invalid value"));
        CHECK_FALSE(TestValue == Copy);
    }

    SECTION("Can construct a complex type using arguments") {
        Retro::TExpected<std::vector<int>, std::error_code> Result(std::in_place, { 1, 2, 3, 4, 5 });
        REQUIRE(Result.IsSet());
        CHECK(*Result == std::vector{ 1, 2, 3, 4, 5 });

        Result = Retro::TExpected<std::vector<int>, std::error_code>(std::in_place, Result->begin(), Result->end());
        REQUIRE(Result.IsSet());
        CHECK(*Result == std::vector{1, 2, 3, 4, 5});
        
        Result.Emplace(std::vector(10, 32));
        REQUIRE(Result.IsSet());
        CHECK(Result == std::vector{32, 32, 32, 32, 32, 32, 32, 32, 32, 32});
    }
    
}
