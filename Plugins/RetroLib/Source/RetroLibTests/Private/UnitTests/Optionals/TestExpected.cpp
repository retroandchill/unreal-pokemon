#include "TestAdapter.h"
#include "RetroLib/Optionals/AndThen.h"
#include "RetroLib/Optionals/Error.h"
#include "RetroLib/Optionals/ErrorPtrOrNull.h"
#include "RetroLib/Optionals/Filter.h"
#include "RetroLib/Optionals/IfNotPresent.h"
#include "RetroLib/Optionals/IfPresent.h"
#include "RetroLib/Optionals/IfPresentOrElse.h"
#include "RetroLib/Optionals/To.h"
#include "RetroLib/Optionals/Transform.h"
#include "RetroLib/Optionals/TransformError.h"

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
        CHECK(Copy.GetPtrOrNull() != TestValue.GetPtrOrNull())

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

    SECTION("Can construct using void as the first parameter") {
        Retro::TExpected<void, std::string> TestValue;
        REQUIRE(TestValue.IsSet());
        CHECK_NOTHROW(TestValue.GetValue())

        TestValue = Retro::TUnexpected(std::string("Invalid value"));
        REQUIRE(!TestValue.IsSet());
        CHECK_THROWS_AS(TestValue.GetValue(), Retro::TBadExpectedAccess<std::string>)

        TestValue.Emplace();
        REQUIRE(TestValue.IsSet());
    }

    SECTION("Can get alternate values/errors") {
        Retro::TExpected<int, std::string> TestValue = 10;
        CHECK(TestValue.Get(14) == 10)
        CHECK(TestValue.GetError("Invalid string") == "Invalid string")

        TestValue = Retro::TUnexpected(std::string("Real error"));
        CHECK(TestValue.Get(14) == 14)
        CHECK(TestValue.GetError("Invalid string") == "Real error")
        
        Retro::TExpected<void, std::string> SecondValue;
        CHECK(SecondValue.GetError("Invalid string") == "Invalid string")

        SecondValue = Retro::TUnexpected(std::string("Real error"));
        CHECK(SecondValue.GetError("Invalid string") == "Real error")
        
    }
}

TEST_CASE_NAMED(FTestExpectedTransform, "Unit Tests::RetroLib::Optionals::Expected::Pipes::Transform", "[RetroLib][Optionals]") {
    SECTION("Can transform the expected using value types") {
        Retro::TExpected<int, std::string> TestValue = 15;
        static_assert(Retro::Optionals::OptionalType<Retro::TExpected<float, std::string>>);
        auto Transformed = TestValue | Retro::Optionals::Transform([](int V) { return std::to_string(V); });
        REQUIRE(Transformed.IsSet());
        CHECK(*Transformed == "15");
    }

    SECTION("Can pass the error of a missing value along") {
        auto Transformed = Retro::TExpected<float, std::string>(Retro::Unexpect, std::string("Invalid value")) |
            Retro::Optionals::Transform([](float V) { return static_cast<int>(V); });
        REQUIRE(!Transformed.IsSet());
        CHECK(Transformed.GetError() == "Invalid value");
    }

    SECTION("Can transform a void expected, getting a simple supplier") {
        auto Transformed = Retro::TExpected<void, std::string>() |
            Retro::Optionals::Transform([] { return 5; });
        REQUIRE(Transformed.IsSet());
        CHECK(Transformed == 5);
    }

    SECTION("Can transform a void expected, getting a simple supplier with an invalid optional") {
        auto Transformed = Retro::TExpected<void, std::string>(Retro::Unexpect, "Invalid") |
            Retro::Optionals::Transform([] { return 5; });
        REQUIRE(!Transformed.IsSet());
        CHECK(Transformed.GetError() == "Invalid");
    }
}

TEST_CASE_NAMED(FTestExpectedFilter, "Unit Tests::RetroLib::Optionals::Expected::Pipes::Filter", "[RetroLib][Optionals]") {
    SECTION("Can filter an expected, default constructing the error") {
        Retro::TExpected<float, std::string> TestValue = 1.5;
        auto ValidFilter = TestValue |
            Retro::Optionals::Filter([](float V) { return V > 1.f; });
        REQUIRE(ValidFilter.IsSet());
        CHECK(ValidFilter == TestValue);

        auto InvalidFilter = TestValue |
            Retro::Optionals::Filter([](float V) { return V < 1.f; });
        REQUIRE(!InvalidFilter.IsSet());
        CHECK(InvalidFilter.GetError().empty());
    }

    SECTION("Can supply an explicit value to use as the default") {
        auto Filtered = Retro::TExpected<float, std::string>(1.5) |
            Retro::Optionals::Filter([](float V) { return V < 1.f; }, "Expected value to be less than 1.5");
        REQUIRE(!Filtered.IsSet());
        CHECK(Filtered.GetError() == "Expected value to be less than 1.5");
    }

    SECTION("Can supply a functor to create the filter") {
        auto Filtered = Retro::TExpected<float, std::string>(1.5) |
            Retro::Optionals::Filter([](float V) { return V < 1.f; }, [] { return "Expected value to be less than 1.5"; });
        REQUIRE(!Filtered.IsSet());
        CHECK(Filtered.GetError() == "Expected value to be less than 1.5");
    }
}

TEST_CASE_NAMED(FTestExpectedIfPresent, "Unit Tests::RetroLib::Optionals::Expected::Pipes::IfPresent", "[RetroLib][Optionals]") {
    SECTION("Can operate on an expected checking if the value is present") {
        Retro::TExpected<int, std::string> TestValue(14);
        int ValueOut = 0;
        TestValue | Retro::Optionals::IfPresent([&ValueOut](int V) { ValueOut = V; });
        CHECK(ValueOut == 14);
    }

    SECTION("Can operate on an expected with a void type") {
        Retro::TExpected<void, std::string> TestValue;
        bool Triggered = false;
        TestValue | Retro::Optionals::IfPresent([&Triggered] { Triggered = true; });
        CHECK(Triggered);
    }

    SECTION("Can execute if a value is not present") {
        int Sum = 0;
        Retro::TExpected<void, int> Value1;
        Value1 | Retro::Optionals::IfNotPresent([&Sum](int Value) { Sum += Value; });
        Retro::TExpected<void, int> Value2 = Retro::TUnexpected(23);
        Value2 | Retro::Optionals::IfNotPresent([&Sum](int Value) { Sum += Value; });
        CHECK(Sum == 23);
    }
}

TEST_CASE_NAMED(FTestExpectedIfPresentOrElse, "Unit Tests::RetroLib::Optionals::Expected::Pipes::IfPresentOrElse", "[RetroLib][Optionals]") {
    SECTION("Can operate on an expected while ignoring the error") {
        Retro::TExpected<float, std::string> TestValue(Retro::Unexpect, "Expected value to be less than 1.5");
        bool Triggered = false;
        TestValue | Retro::Optionals::IfPresentOrElse([](float) { }, [&Triggered] { Triggered = true; });
        CHECK(Triggered);
    }

    SECTION("Can operate on an expected checking if the value is present") {
        Retro::TExpected<int, std::string> TestValue(14);
        int ValueOut = 0;
        TestValue | Retro::Optionals::IfPresentOrElse([&ValueOut](int V) { ValueOut = V; }, [] {});
        CHECK(ValueOut == 14);
    }

    SECTION("Can operate on an expected while considering the error") {
        Retro::TExpected<std::string, int> TestValue(Retro::Unexpect, 42);
        int ErrorCode = 0;
        TestValue | Retro::Optionals::IfPresentOrElse([](std::string&) { }, [&ErrorCode](int EC) { ErrorCode = EC; });
        CHECK(ErrorCode == 42);
    }

    SECTION("Can operate on an expected with a void type") {
        Retro::TExpected<void, std::string> TestValue;
        bool Triggered = false;
        TestValue | Retro::Optionals::IfPresentOrElse([&Triggered] { Triggered = true; }, [] {});
        CHECK(Triggered);
    }

    SECTION("Can operate on a void expected while ignoring the error") {
        Retro::TExpected<void, std::string> TestValue(Retro::Unexpect, "Expected value to be less than 1.5");
        bool Triggered = false;
        TestValue | Retro::Optionals::IfPresentOrElse([] { }, [&Triggered] { Triggered = true; });
        CHECK(Triggered);
    }

    SECTION("Can operate on a void expected while considering the error") {
        Retro::TExpected<void, int> TestValue(Retro::Unexpect, 42);
        int ErrorCode = 0;
        TestValue | Retro::Optionals::IfPresentOrElse([] { }, [&ErrorCode](int EC) { ErrorCode = EC; });
        CHECK(ErrorCode == 42);
    }
}

TEST_CASE_NAMED(FTestExpectedAndThen, "Unit Tests::RetroLib::Optionals::Expected::Pipes::AndThen", "[RetroLib][Optionals]") {
    SECTION("Can convert an expected into an optional") {
        Retro::TExpected<int, std::string> TestValue(14);
        auto AsOptional = TestValue |
            Retro::Optionals::AndThen([](int V) { return std::optional(V * 2); });
        CHECK(AsOptional == 28)
    }

    SECTION("Can convert an expected to another expected") {
        Retro::TExpected<int, std::string> TestValue(14);
        auto AsExpected = TestValue |
            Retro::Optionals::AndThen([](int V) { return Retro::TExpected<int, std::string_view>(V * 2); });
        CHECK(AsExpected == 28)
    }

    SECTION("Can convert an expected to another expected keeping the error") {
        Retro::TExpected<int, std::string> TestValue(Retro::Unexpect, "Invalid value");
        auto AsExpected = TestValue |
            Retro::Optionals::AndThen([](int V) { return Retro::TExpected<int, std::string_view>(V * 2); });
        REQUIRE(!AsExpected.IsSet())
        CHECK(AsExpected.GetError() == "Invalid value");
    }

    SECTION("Can convert an optional to an expected, constructing the error type if needed") {
        std::optional Optional = 4;
        auto AsExpected = Optional |
            Retro::Optionals::AndThen([](int V) { return Retro::TExpected<int, std::string_view>(V * 2); }, "Unused error");
        CHECK(AsExpected.IsSet())
    }

    SECTION("Can convert an optional to an expected, constructing the error type on demand") {
        std::optional<int> Optional;
        auto AsExpected = Optional |
            Retro::Optionals::AndThen([](int V) { return Retro::TExpected<int, std::string_view>(V * 2); }, [] { return "Constructed error"; });
        REQUIRE(!AsExpected.IsSet())
        CHECK(AsExpected.GetError() == "Constructed error");
    }
}

TEST_CASE_NAMED(FTestExpectedTo, "Unit Tests::RetroLib::Optionals::Expected::Pipes::To", "[RetroLib][Optionals]") {
    SECTION("Can convert an expected into an optional") {
        Retro::TExpected<int, std::string> TestValue(14);
        auto AsOptional = TestValue |
            Retro::Optionals::To<std::optional>();
        CHECK(AsOptional == 14)
    }

    SECTION("Can convert an expected to another expected") {
        Retro::TExpected<int, std::string> TestValue(14);
        auto AsExpected = TestValue |
            Retro::Optionals::To<Retro::TExpected<int, std::string_view>>();
        CHECK(AsExpected == 14)
    }

    SECTION("Can convert an expected to another expected keeping the error") {
        Retro::TExpected<int, std::string> TestValue(Retro::Unexpect, "Invalid value");
        auto AsExpected = TestValue |
            Retro::Optionals::To<Retro::TExpected, std::string_view>();
        REQUIRE(!AsExpected.IsSet())
        CHECK(AsExpected.GetError() == "Invalid value");
    }

    SECTION("Can convert a void expected to another void expected keeping the error") {
        Retro::TExpected<void, std::string> TestValue(Retro::Unexpect, "Invalid value");
        auto AsExpected = TestValue |
            Retro::Optionals::To<Retro::TExpected, std::string_view>();
        REQUIRE(!AsExpected.IsSet())
        CHECK(AsExpected.GetError() == "Invalid value");
    }

    SECTION("Can convert an optional to an expected, constructing the error type if needed") {
        std::optional Optional = 4;
        auto AsExpected = Optional |
            Retro::Optionals::To<Retro::TExpected<int, std::string_view>>("Unused error");
        CHECK(AsExpected.IsSet())
    }

    SECTION("Can convert an optional to an expected, constructing the error type on demand") {
        std::optional<int> Optional;
        auto AsExpected = Optional |
            Retro::Optionals::To<Retro::TExpected, std::string>([] { return "Constructed error"; });
        REQUIRE(!AsExpected.IsSet())
        CHECK(AsExpected.GetError() == "Constructed error");
    }
}

TEST_CASE_NAMED(FTestTransformError, "Unit Tests::RetroLib::Optionals::TransformError", "[RetroLib][Optionals]") {
    SECTION("Can transform the error of an expected") {
        Retro::TExpected<int, std::string> TestValue(Retro::Unexpect, "Invalid value");
        auto Transformed = TestValue |
            Retro::Optionals::TransformError([](const std::string &V) { return V.size(); });
        REQUIRE(!Transformed.IsSet())
        CHECK(Transformed.GetError() == 13);
    }

    SECTION("Can transform the error of an expected") {
        Retro::TExpected<int, std::string> TestValue(3);
        auto Transformed = TestValue |
            Retro::Optionals::TransformError([](const std::string &V) { return V.size(); });
        REQUIRE(Transformed.IsSet())
        CHECK(*Transformed == 3);
    }
}

TEST_CASE_NAMED(FTestExpectedErrorGetter, "Unit Tests::RetroLib::Optionals::Expected::Error", "[RetroLib][Optionals]") {
    SECTION("Can get the error of an expected") {
        Retro::TExpected<int, std::string> TestValue(Retro::Unexpect, "Invalid value");
        REQUIRE(Retro::Optionals::ErrorPtrOrNull(TestValue) != nullptr)
        CHECK(Retro::Optionals::Error(TestValue) == "Invalid value")

        TestValue.Emplace(10);
        CHECK(Retro::Optionals::ErrorPtrOrNull(TestValue) == nullptr)
    }
}