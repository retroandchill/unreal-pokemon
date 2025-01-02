/**
 * @file TestFilterOptional.cpp
 * @brief Test for the optional filter operation.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#include "TestAdapter.h"

#if RETROLIB_WITH_MODULES
import RetroLib;
import std;
#else
#include "RetroLib.h"

#include <array>
#endif

namespace Retro::Testing::Optionals {
    struct DataStruct {
        int Member;
    };
}

TEST_CASE_NAMED(FFilterOptionalTest, "RetroLib::Optionals::Filter", "[optionals]") {
    SECTION("Can filter on a constant functor") {
        constexpr auto IsEven = [](int X, int Y) { return X % Y == 0; };
        std::optional Odd = 3;
        std::optional Even = 4;
        std::optional<int> Empty;
        auto FilteredOdd = Odd | Retro::Optionals::Filter<IsEven>(2);
        CHECK_FALSE(FilteredOdd.has_value());
        auto FilteredEven = Even | Retro::Optionals::Filter<IsEven>(2);
        CHECK(FilteredEven.has_value());
        auto FilteredEmpty = Empty | Retro::Optionals::Filter<IsEven>(2);
        CHECK_FALSE(FilteredEmpty.has_value());

        auto FilteredRvalueOdd = std::optional(5) | Retro::Optionals::Filter<IsEven>(2);
        CHECK_FALSE(FilteredRvalueOdd.has_value());
        auto FilteredRvalueEven = std::optional(6) | Retro::Optionals::Filter<IsEven>(2);
        CHECK(FilteredRvalueEven.has_value());
        auto FilteredEmptyRvalue = std::optional<int>() | Retro::Optionals::Filter<IsEven>(2);
        CHECK_FALSE(FilteredEmptyRvalue.has_value());
    }

    SECTION("Can filter on a runtime functor") {
        auto GreaterThan = [](int x, int y) { return x > y; };
        std::optional Odd = 3;
        std::optional Even = 4;
        std::optional<int> Empty;
        static_assert(Retro::Optionals::OptionalType<decltype(Odd)>);
        auto FilteredOdd = Odd | Retro::Optionals::Filter(GreaterThan, 4);
        CHECK_FALSE(FilteredOdd.has_value());
        auto FilteredEven = Even | Retro::Optionals::Filter(GreaterThan, 3);
        CHECK(FilteredEven.has_value());
        auto FilteredEmpty = Empty | Retro::Optionals::Filter(GreaterThan, 3);
        CHECK_FALSE(FilteredEmpty.has_value());

        auto FilteredRvalueOdd = std::optional(5) | Retro::Optionals::Filter(GreaterThan, 7);
        CHECK_FALSE(FilteredRvalueOdd.has_value());
        auto FilteredRvalueEven = std::optional(6) | Retro::Optionals::Filter(GreaterThan, 4);
        CHECK(FilteredRvalueEven.has_value());
        auto FilteredRvalueEmpty = std::optional<int>() | Retro::Optionals::Filter(GreaterThan, 4);
        CHECK_FALSE(FilteredRvalueEmpty.has_value());
    }
}

TEST_CASE_NAMED(FTransformOptionalTest, "RetroLib::Optionals::Transform", "[optionals]") {
    constexpr auto MultiplyValue = [](int x, int y) { return x * y; };
    SECTION("Can transform using a constant functor") {
        std::optional Value = 3;
        auto Transformed = Value | Retro::Optionals::Transform<MultiplyValue>(2);
        CHECK(Transformed.has_value());
        CHECK(Transformed.value() == 6);

        std::optional<int> EmptyOpt;
        auto EmptyTransformed = EmptyOpt | Retro::Optionals::Transform<MultiplyValue>(2);
        CHECK_FALSE(EmptyTransformed.has_value());
    }

    SECTION("Can transform using a runtime defined functor") {
        std::optional Value = 6;
        auto Transformed = Value | Retro::Optionals::Transform(MultiplyValue, 5);
        CHECK(Transformed.has_value());
        CHECK(Transformed.value() == 30);
    }

    SECTION("Can transform a value returning a reference to hold said reference") {
        std::array Values = {1, 2, 3, 4, 5};
        std::optional Index = 4;
        auto Transformed =
            Index | Retro::Optionals::Transform([&Values](int i) -> decltype(auto) { return Values[i]; });
        CHECK(Transformed.has_value());
        CHECK(Transformed.value() == 5);
    }

    SECTION("Can transform a value returning a pointer to hold a reference to that pointer") {
        std::array Values = {1, 2, 3, 4, 5};
        auto Transformer = [&Values](int i) {
            if (Values.size() > i) {
                return &Values[i];
            }

            return static_cast<int *>(nullptr);
        };
        std::optional Index = 2;
        auto Transformed = Index | Retro::Optionals::Transform(Transformer);
        CHECK(Transformed.has_value());
        CHECK(Transformed.value() == 3);

        Index = 6;
        auto SecondPass = Index | Retro::Optionals::Transform(Transformer);
        CHECK_FALSE(SecondPass.has_value());
    }
}

TEST_CASE_NAMED(FAndThenOptionalTest, "RetroLib::Optionals::AndThen", "[optionals]") {
    SECTION("Can return an optional of the same type") {
        constexpr auto Mapper = [](int x) {
            if (x > 0) {
                return std::optional(x * 2);
            }

            return std::optional<int>();
        };

        auto Mapped1 = std::optional(4) | Retro::Optionals::AndThen(Mapper);
        CHECK(Mapped1.has_value());
        CHECK(Mapped1.value() == 8);

        auto Mapped2 = std::optional(-3) | Retro::Optionals::AndThen(Mapper);
        CHECK_FALSE(Mapped2.has_value());

        auto Mapped3 = std::optional<int>() | Retro::Optionals::AndThen(Mapper);
        CHECK_FALSE(Mapped3.has_value());
    }

    SECTION("Can return an optional of a different type") {
        constexpr auto Mapper = [](int x) {
            if (x > 0) {
                return std::optional(x * 2);
            }

            return std::optional<int>();
        };

        auto Mapped1 = std::optional(4) | Retro::Optionals::AndThen<Mapper>();
        CHECK(Mapped1.has_value());
        CHECK(Mapped1.value() == 8);

        auto Mapped2 = std::optional(-3) | Retro::Optionals::AndThen<Mapper>();
        CHECK_FALSE(Mapped2.has_value());

        auto Mapped3 = std::optional<int>() | Retro::Optionals::AndThen<Mapper>();
        CHECK_FALSE(Mapped3.has_value());
    }
}

TEST_CASE_NAMED(FOptionalOrElseThrowTest, "RetroLib::Optionals::OrElseThrow", "[optionals]") {
    std::optional Value1 = 4;
    CHECK_NOTHROW(Value1 | Retro::Optionals::OrElseThrow());

    std::optional<int> Value2 = std::nullopt;
    CHECK_THROWS_AS(Value2 | Retro::Optionals::OrElseThrow(), std::bad_optional_access);

    constexpr auto AltThrow = [](std::string_view Message) { return std::runtime_error(Message.data()); };
    CHECK_THROWS_AS(Value2 | Retro::Optionals::OrElseThrow<AltThrow>("Could not get value!"), std::runtime_error);
    CHECK_THROWS_AS(Value2 | Retro::Optionals::OrElseThrow(AltThrow, "Could not get value!"), std::runtime_error);
}

TEST_CASE_NAMED(FOptionalGetTest, "RetroLib::Optionals::Value", "[optionals]") {
    std::optional Value1 = 4;
    auto Result = Value1 | Retro::Optionals::Value;
    CHECK(Result == 4);
}

TEST_CASE_NAMED(FOptionalPtrOrNullTest, "RetroLib::Optionals::PtrOrNull", "[optionals]") {
    std::optional Value1 = 4;
    auto Result1 = Value1 | Retro::Optionals::PtrOrNull;
    REQUIRE(Result1 != nullptr);
    CHECK(*Result1 == 4);

    std::optional<int> Value2 = std::nullopt;
    auto Result2 = Value2 | Retro::Optionals::PtrOrNull;
    CHECK(Result2 == nullptr);

    auto Result3 = Retro::Optionals::MakeOptionalReference(Value2) | Retro::Optionals::PtrOrNull;
    CHECK(Result3 == nullptr);

    auto Result4 = Retro::Optionals::OfNullable(Result1) | Retro::Optionals::PtrOrNull;
    CHECK(Result4 == Result1);
}

TEST_CASE_NAMED(FOptionalIsSetTest, "RetroLib::Optionals::IsSet", "[optionals]") {
    std::optional Value1 = 4;
    CHECK(Value1 | Retro::Optionals::IsSet);

    std::optional<int> Value2 = std::nullopt;
    CHECK_FALSE(Value2 | Retro::Optionals::IsSet);
}

#ifdef __UNREAL__
TEST_CASE_NAMED(FOptionalToTest, "RetroLib::Optionals::To", "[optionals]") {
    SECTION("Can convert between two optionals holding the same parameter") {
        std::optional Value1 = 34;
        auto Value2 = Value1 | Retro::Optionals::To<TOptional>();
        CHECK(Value2.IsSet());
        CHECK(Value2.GetValue() == 34);

        std::optional<int> Value3 = std::nullopt;
        auto Value4 = Value3 | Retro::Optionals::To<TOptional>();
        CHECK_FALSE(Value4.IsSet());
    }

    SECTION("Can convert between two unlike optional types") {
        std::optional Value1 = 34;
        auto Value2 = Value1 | Retro::Optionals::To<TOptional<double>>();
        CHECK(Value2.IsSet());
        CHECK(Value2.GetValue() == 34.0);

        std::optional<int> Value3 = std::nullopt;
        auto Value4 = Value3 | Retro::Optionals::To<TOptional<double>>();
        CHECK_FALSE(Value4.IsSet());
    }

    SECTION("Can convert from a reference-wrapped optional to a raw reference optional") {
        int RefValue = 34;
        std::optional Value1 = std::ref(RefValue);
        auto Value2 = Value1 | Retro::Optionals::To<TOptional>();
        CHECK(Value2.IsSet());
        CHECK(Value2.GetValue() == 34);

        std::optional<std::reference_wrapper<int>> Value3 = std::nullopt;
        auto Value4 = Value3 | Retro::Optionals::To<TOptional>();
        CHECK_FALSE(Value4.IsSet());
    }

    SECTION("Can convert from a raw reference optional to a reference-wrapped optional") {
        int RefValue = 34;
        TOptional<int &> Value1 = RefValue;
        auto Value2 = Value1 | Retro::Optionals::To<std::optional>();
        CHECK(Value2.has_value());
        CHECK(Value2.value() == 34);

        TOptional<int&> Value3;
        auto Value4 = Value3 | Retro::Optionals::To<std::optional>();
        CHECK_FALSE(Value4.has_value());
    }
}
#endif

TEST_CASE_NAMED(FOptionalOrElseValueTest, "RetroLib::Optionals::OrElseValue", "[optionals]") {
    SECTION("Can get the value of a value type out") {
        std::optional Value1 = 34;
        auto Value2 = Value1 | Retro::Optionals::OrElseGet([] { return 5; });
        CHECK(Value2 == 34);

        std::optional<int> Value3 = std::nullopt;
        auto Value4 = Value3 | Retro::Optionals::OrElseGet([] { return 5; });
        CHECK(Value4 == 5);
    }

    SECTION("Can get the value of a reference type out") {
        int RefValue = 34;
        int AltValue = 45;
        std::optional<std::reference_wrapper<int>> Value1 = RefValue;
        decltype(auto) Value2 = Value1 | Retro::Optionals::OrElseGet([&AltValue]() -> int & { return AltValue; });
        CHECK(Value2 == 34);

        std::optional<std::reference_wrapper<int>> Value3 = std::nullopt;
        decltype(auto) Value4 = Value3 | Retro::Optionals::OrElseGet([&AltValue]() -> int & { return AltValue; });
        CHECK(Value4 == 45);
    }

    SECTION("Can collapse two different types") {
        int RefValue = 34;
        std::optional<std::reference_wrapper<int>> Value1 = RefValue;
        decltype(auto) Value2 = Value1 | Retro::Optionals::OrElseGet([] { return 50.0; });
        CHECK(Value2 == 34);

        std::optional<std::reference_wrapper<int>> Value3 = std::nullopt;
        decltype(auto) Value4 = Value3 | Retro::Optionals::OrElseGet([] { return 50.0; });
        CHECK(Value4 == 50.0);
    }

    SECTION("Can get an optional out of the call") {
        std::optional Value1 = 34;
        auto Value2 = Value1 | Retro::Optionals::OrElse([] { return std::optional(5); });
        REQUIRE(Value2.has_value());
        CHECK(*Value2 == 34);

        std::optional<int> Value3 = std::nullopt;
        auto Value4 = Value3 | Retro::Optionals::OrElse([] { return std::optional(5); });
        REQUIRE(Value4.has_value());
        CHECK(*Value4 == 5);
    }
}

TEST_CASE_NAMED(FOptionalOrElseTest, "RetroLib::Optionals::OrElse", "[optionals]") {
    SECTION("Can get basic values out") {
        std::optional Value1 = 34;
        auto Value2 = Value1 | Retro::Optionals::OrElseValue(5);
        CHECK(Value2 == 34);

        std::optional<int> Value3 = std::nullopt;
        auto Value4 = Value3 | Retro::Optionals::OrElseValue(5);
        CHECK(Value4 == 5);
    }

    SECTION("Can get references out") {
        int RefValue = 34;
        int AltValue = 45;
        std::optional<std::reference_wrapper<int>> Value1 = RefValue;
        decltype(auto) Value2 = Value1 | Retro::Optionals::OrElseValue(std::ref(AltValue));
        CHECK(Value2 == 34);

        std::optional<std::reference_wrapper<int>> Value3 = std::nullopt;
        decltype(auto) Value4 = Value3 | Retro::Optionals::OrElseValue(std::ref(AltValue));
        CHECK(Value4 == AltValue);
    }
}

TEST_CASE_NAMED(FOptionalIfPresentTest, "RetroLib::Optionals::IfPresent", "[optionals]") {
    SECTION("Can execute if a value is present of do nothing") {
        int Sum = 0;
        std::optional Value1 = 34;
        Value1 | Retro::Optionals::IfPresent([&Sum](int Value) { Sum += Value; });
        std::optional<int> Value2 = std::nullopt;
        Value2 | Retro::Optionals::IfPresent([&Sum](int Value) { Sum += Value; });
        CHECK(Sum == 34);
    }

    SECTION("Can execute if a value is present, otherwise doing an alternate action") {
        int Sum = 0;
        std::optional Value1 = 34;
        Value1 | Retro::Optionals::IfPresentOrElse([&Sum](int Value) { Sum += Value; }, [&Sum] { Sum += 5; });
        std::optional<int> Value2 = std::nullopt;
        Value2 | Retro::Optionals::IfPresentOrElse([&Sum](int Value) { Sum += Value; }, [&Sum] { Sum += 5; });
        CHECK(Sum == 39);
    }
}

TEST_CASE_NAMED(FOptionalMemberReferenceTest, "RetroLib::Optionals::MemberReference", "[optionals]") {
    using namespace Retro::Testing::Optionals;

    SECTION("Can use the member as a runtime pointer") {
       std::optional Value = DataStruct{3};

        auto Result = Value |
            Retro::Optionals::Transform(&DataStruct::Member);
        CHECK(Result.has_value());
        CHECK(Result.value() == 3);
    }

    SECTION("Can use the member as a compile time pointer") {
        std::optional Value = DataStruct{3};

        auto Result = Value |
            Retro::Optionals::Transform<&DataStruct::Member>();
        CHECK(Result.has_value());
        CHECK(Result.value() == 3);
    }
}