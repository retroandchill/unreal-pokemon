/**
 * @file TestFilterOptional.cpp
 * @brief Test for the optional filter operation.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#include <catch2/catch_test_macros.hpp>

#if RETROLIB_WITH_MODULES
import std;
import RetroLib;
#else
#include "RetroLib/Functional/BindBack.h"
#include "RetroLib/Optionals/AndThen.h"
#include "RetroLib/Optionals/Filter.h"
#include "RetroLib/Optionals/Transform.h"
#include "RetroLib/TypeTraits.h"
#endif

TEST_CASE("Can filter an optional value", "[optionals]") {
    SECTION("Can filter on a constant functor") {
        constexpr auto is_even = [](int x, int y) { return x % y == 0; };
        std::optional odd = 3;
        std::optional even = 4;
        std::optional<int> empty;
        auto filtered_odd = odd | retro::optionals::filter<is_even>(2);
        CHECK_FALSE(filtered_odd.has_value());
        auto filtered_even = even | retro::optionals::filter<is_even>(2);
        CHECK(filtered_even.has_value());
        auto filtered_empty = empty | retro::optionals::filter<is_even>(2);
        CHECK_FALSE(filtered_empty.has_value());

        auto filtered_rvalue_odd = std::optional(5) | retro::optionals::filter<is_even>(2);
        CHECK_FALSE(filtered_rvalue_odd.has_value());
        auto filtered_rvalue_even = std::optional(6) | retro::optionals::filter<is_even>(2);
        CHECK(filtered_rvalue_even.has_value());
        auto filtered_empty_rvalue = std::optional<int>() | retro::optionals::filter<is_even>(2);
        CHECK_FALSE(filtered_empty_rvalue.has_value());
    }

    SECTION("Can filter on a runtime functor") {
        auto greater_than = [](int x, int y) { return x > y; };
        std::optional odd = 3;
        std::optional even = 4;
        std::optional<int> empty;
        static_assert(retro::optionals::OptionalType<decltype(odd)>);
        auto filtered_odd = odd | retro::optionals::filter(greater_than, 4);
        CHECK_FALSE(filtered_odd.has_value());
        auto filtered_even = even | retro::optionals::filter(greater_than, 3);
        CHECK(filtered_even.has_value());
        auto filtered_empty = empty | retro::optionals::filter(greater_than, 3);
        CHECK_FALSE(filtered_empty.has_value());

        auto filtered_rvalue_odd = std::optional(5) | retro::optionals::filter(greater_than, 7);
        CHECK_FALSE(filtered_rvalue_odd.has_value());
        auto filtered_rvalue_even = std::optional(6) | retro::optionals::filter(greater_than, 4);
        CHECK(filtered_rvalue_even.has_value());
        auto filtered_rvalue_empty = std::optional<int>() | retro::optionals::filter(greater_than, 4);
        CHECK_FALSE(filtered_rvalue_empty.has_value());
    }
}

TEST_CASE("Can transform an optional between various types", "[optionals]") {
    constexpr auto multiply_value = [](int x, int y) { return x * y; };
    SECTION("Can transform using a constant functor") {
        std::optional value = 3;
        auto transformed = value | retro::optionals::transform<multiply_value>(2);
        CHECK(transformed.has_value());
        CHECK(transformed.value() == 6);

        std::optional<int> empty_opt;
        auto empty_transformed = empty_opt | retro::optionals::transform<multiply_value>(2);
        CHECK_FALSE(empty_transformed.has_value());
    }

    SECTION("Can transform using a runtime defined functor") {
        std::optional value = 6;
        auto transformed = value | retro::optionals::transform(multiply_value, 5);
        CHECK(transformed.has_value());
        CHECK(transformed.value() == 30);
    }

    SECTION("Can transform a value returning a reference to hold said reference") {
        std::array values = {1, 2, 3, 4, 5};
        std::optional index = 4;
        auto transformed =
            index | retro::optionals::transform([&values](int i) -> decltype(auto) { return values[i]; });
        CHECK(transformed.has_value());
        CHECK(transformed.value() == 5);
    }

    SECTION("Can transform a value returning a pointer to hold a reference to that pointer") {
        std::array values = {1, 2, 3, 4, 5};
        auto transformer = [&values](int i) {
            if (values.size() > i) {
                return &values[i];
            }

            return static_cast<int *>(nullptr);
        };
        retro::Optional index = 2;
        auto transformed = index | retro::optionals::transform(transformer);
        CHECK(transformed.has_value());
        CHECK(transformed.value() == 3);

        index = 6;
        auto second_pass = index | retro::optionals::transform(transformer);
        CHECK_FALSE(second_pass.has_value());
    }
}

TEST_CASE("Can flat map down an optional result", "[optionals]") {
    SECTION("Can return an optional of the same type") {
        constexpr auto mapper = [](int x) {
            if (x > 0) {
                return std::optional(x * 2);
            }

            return std::optional<int>();
        };

        auto mapped1 = std::optional(4) | retro::optionals::and_then(mapper);
        CHECK(mapped1.has_value());
        CHECK(mapped1.value() == 8);

        auto mapped2 = std::optional(-3) | retro::optionals::and_then(mapper);
        CHECK_FALSE(mapped2.has_value());

        auto mapped3 = std::optional<int>() | retro::optionals::and_then(mapper);
        CHECK_FALSE(mapped3.has_value());
    }

    SECTION("Can return an optional of a different type") {
        constexpr auto mapper = [](int x) {
            if (x > 0) {
                return retro::Optional(x * 2);
            }

            return retro::Optional<int>();
        };

        auto mapped1 = std::optional(4) | retro::optionals::and_then<mapper>();
        CHECK(mapped1.has_value());
        CHECK(mapped1.value() == 8);

        auto mapped2 = std::optional(-3) | retro::optionals::and_then<mapper>();
        CHECK_FALSE(mapped2.has_value());

        auto mapped3 = std::optional<int>() | retro::optionals::and_then<mapper>();
        CHECK_FALSE(mapped3.has_value());
    }
}

TEST_CASE("Test getting a value or throwing an exception if its empty", "[optionals]") {
    std::optional value1 = 4;
    CHECK_NOTHROW(value1 | retro::optionals::or_else_throw());

    std::optional<int> value2 = std::nullopt;
    CHECK_THROWS_AS(value2 | retro::optionals::or_else_throw(), std::bad_optional_access);

    constexpr auto alt_throw = [](std::string_view message) { return std::runtime_error(message.data()); };
    CHECK_THROWS_AS(value2 | retro::optionals::or_else_throw<alt_throw>("Could not get value!"), std::runtime_error);
    CHECK_THROWS_AS(value2 | retro::optionals::or_else_throw(alt_throw, "Could not get value!"), std::runtime_error);
}

TEST_CASE("Test getting a value without any runtime checks (very unsafe)", "[optionals]") {
    std::optional value1 = 4;
    auto result = value1 | retro::optionals::value;
    CHECK(result == 4);
}

TEST_CASE("Test getting a value or a null pointer", "[optionals]") {
    std::optional value1 = 4;
    auto result1 = value1 | retro::optionals::ptr_or_null;
    REQUIRE(result1 != nullptr);
    CHECK(*result1 == 4);

    std::optional<int> value2 = std::nullopt;
    auto result2 = value2 | retro::optionals::ptr_or_null;
    CHECK(result2 == nullptr);

    auto result3 = retro::optionals::make_optional_reference(value2) | retro::optionals::ptr_or_null;
    CHECK(result3 == nullptr);

    auto result4 = retro::optionals::of_nullable(result1) | retro::optionals::ptr_or_null;
    CHECK(result4 == result1);
}

TEST_CASE("Can check if an optional value is set", "[optionals]") {
    std::optional value1 = 4;
    CHECK(value1 | retro::optionals::is_set);

    std::optional<int> value2 = std::nullopt;
    CHECK_FALSE(value2 | retro::optionals::is_set);
}

TEST_CASE("Can convert between various optional types", "[optionals]") {
    SECTION("Can convert between two optionals holding the same parameter") {
        std::optional value1 = 34;
        auto value2 = value1 | retro::optionals::to<retro::Optional>();
        CHECK(value2.has_value());
        CHECK(value2.value() == 34);

        std::optional<int> value3 = std::nullopt;
        auto value4 = value3 | retro::optionals::to<retro::Optional>();
        CHECK_FALSE(value4.has_value());
    }

    SECTION("Can convert between two unlike optional types") {
        std::optional value1 = 34;
        auto value2 = value1 | retro::optionals::to<retro::Optional<double>>();
        CHECK(value2.has_value());
        CHECK(value2.value() == 34.0);

        std::optional<int> value3 = std::nullopt;
        auto value4 = value3 | retro::optionals::to<retro::Optional<double>>();
        CHECK_FALSE(value4.has_value());
    }

    SECTION("Can convert from a reference-wrapped optional to a raw reference optional") {
        int ref_value = 34;
        std::optional value1 = std::ref(ref_value);
        auto value2 = value1 | retro::optionals::to<retro::Optional>();
        CHECK(value2.has_value());
        CHECK(value2.value() == 34);

        std::optional<std::reference_wrapper<int>> value3 = std::nullopt;
        auto value4 = value3 | retro::optionals::to<retro::Optional>();
        CHECK_FALSE(value4.has_value());
    }

    SECTION("Can convert from a raw reference optional to a reference-wrapped optional") {
        int ref_value = 34;
        retro::Optional<int &> value1 = ref_value;
        auto value2 = value1 | retro::optionals::to<std::optional>();
        CHECK(value2.has_value());
        CHECK(value2.value() == 34);

        std::optional<std::reference_wrapper<int>> value3 = std::nullopt;
        auto value4 = value3 | retro::optionals::to<std::optional>();
        CHECK_FALSE(value4.has_value());
    }
}

TEST_CASE("Can get the value inside of an optional or an alternative", "[optionals]") {
    SECTION("Can get the value of a value type out") {
        std::optional value1 = 34;
        auto value2 = value1 | retro::optionals::or_else_get([] { return 5; });
        CHECK(value2 == 34);

        std::optional<int> value3 = std::nullopt;
        auto value4 = value3 | retro::optionals::or_else_get([] { return 5; });
        CHECK(value4 == 5);
    }

    SECTION("Can get the value of a reference type out") {
        int ref_value = 34;
        int alt_value = 45;
        retro::Optional<int &> value1 = ref_value;
        decltype(auto) value2 = value1 | retro::optionals::or_else_get([&alt_value]() -> int & { return alt_value; });
        CHECK(value2 == 34);

        retro::Optional<int &> value3 = std::nullopt;
        decltype(auto) value4 = value3 | retro::optionals::or_else_get([&alt_value]() -> int & { return alt_value; });
        CHECK(value4 == 45);
    }

    SECTION("Can collapse two different types") {
        int ref_value = 34;
        retro::Optional<int &> value1 = ref_value;
        decltype(auto) value2 = value1 | retro::optionals::or_else_get([] { return 50.0; });
        CHECK(value2 == 34);

        retro::Optional<int &> value3 = std::nullopt;
        decltype(auto) value4 = value3 | retro::optionals::or_else_get([] { return 50.0; });
        CHECK(value4 == 50.0);
    }

    SECTION("Can get an optional out of the call") {
        std::optional value1 = 34;
        auto value2 = value1 | retro::optionals::or_else([] { return std::optional(5); });
        REQUIRE(value2.has_value());
        CHECK(*value2 == 34);

        std::optional<int> value3 = std::nullopt;
        auto value4 = value3 | retro::optionals::or_else([] { return std::optional(5); });
        REQUIRE(value4.has_value());
        CHECK(*value4 == 5);
    }
}

TEST_CASE("Can get the value or an alternative", "[optionals]") {
    SECTION("Can get basic values out") {
        std::optional value1 = 34;
        auto value2 = value1 | retro::optionals::or_else_value(5);
        CHECK(value2 == 34);

        std::optional<int> value3 = std::nullopt;
        auto value4 = value3 | retro::optionals::or_else_value(5);
        CHECK(value4 == 5);
    }

    SECTION("Can get references out") {
        int ref_value = 34;
        int alt_value = 45;
        retro::Optional<int &> value1 = ref_value;
        decltype(auto) value2 = value1 | retro::optionals::or_else_value(std::ref(alt_value));
        CHECK(value2 == 34);

        retro::Optional<int &> value3 = std::nullopt;
        decltype(auto) value4 = value3 | retro::optionals::or_else_value(std::ref(alt_value));
        CHECK(value4 == alt_value);
    }
}

TEST_CASE("Can execute on a value if said value is present", "[optionals]") {
    SECTION("Can execute if a value is present of do nothing") {
        int sum = 0;
        std::optional value1 = 34;
        value1 | retro::optionals::if_present([&sum](int value) { sum += value; });
        std::optional<int> value2 = std::nullopt;
        value2 | retro::optionals::if_present([&sum](int value) { sum += value; });
        CHECK(sum == 34);
    }

    SECTION("Can execute if a value is present, otherwise doing an alternate action") {
        int sum = 0;
        std::optional value1 = 34;
        value1 | retro::optionals::if_present_or_else([&sum](int value) { sum += value; }, [&sum] { sum += 5; });
        std::optional<int> value2 = std::nullopt;
        value2 | retro::optionals::if_present_or_else([&sum](int value) { sum += value; }, [&sum] { sum += 5; });
        CHECK(sum == 39);
    }
}