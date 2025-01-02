
#include "RetroLib.h"
#include "TestAdapter.h"

#include <array>

TEST_CASE_NAMED(FRangePipeTest, "RetroLib::Ranges::Compatibility", "[RetroLib][Ranges]") {
    SECTION("Can iterate over an instance of TArray") {
        static_assert(std::ranges::contiguous_range<TArray<int32>>);
        TArray Container = {1, 2, 3, 4, 5};
        auto Transformed = Container | Retro::Ranges::Views::Transform([](int32 I) { return I * 2; });

        int32 Sum = 0;
        for (auto I : Transformed) {
            Sum += I;
        }
        CHECK(Sum == 30);
    }

    SECTION("Can iterate over an instance of TArrayView") {
        static_assert(std::ranges::contiguous_range<TArrayView<int32>>);
        TArray Container = {1, 2, 3, 4, 5};
        TArrayView<int32> View = Container;
        auto Transformed = View | Retro::Ranges::Views::Transform([](int32 I) { return I * 2; });

        int32 Sum = 0;
        for (auto I : Transformed) {
            Sum += I;
        }
        CHECK(Sum == 30);
    }

    SECTION("Can iterate over an instance of TSet") {
        static_assert(std::ranges::input_range<TSet<int32>>);
        TSet Container = {1, 2, 3, 4, 5};
        auto Transformed = Container | Retro::Ranges::Views::Transform([](int32 I) { return I * 2; });

        int32 Sum = 0;
        for (auto I : Transformed) {
            Sum += I;
        }
        CHECK(Sum == 30);
    }

    SECTION("Can iterate over an instance of TMap") {
        static_assert(std::ranges::input_range<TMap<int32, int32>>);
        static_assert(Retro::TupleLike<std::ranges::range_value_t<TMap<int32, int32>>>);
        TMap<int32, int32> Container = {{1, 2}, {3, 4}, {5, 6}};
        auto Transformed =
            Container | Retro::Ranges::Views::Values | Retro::Ranges::Views::Transform([](int32 I) { return I * 2; });

        int32 Sum = 0;
        for (auto I : Transformed) {
            Sum += I;
        }
        CHECK(Sum == 24);
    }
}

TEST_CASE_NAMED(FRangesToTest, "RetroLib::Ranges::To", "[RetroLib][Ranges]") {
    SECTION("Can convert a TArray to a TSet") {
        TArray Array = {1, 2, 3, 4, 5};
        auto Set = Array | Retro::Ranges::To<TSet>();
        CHECK(Set.Num() == 5);
    }

    SECTION("Can convert a vector to a TArray") {
        std::vector Vector = {1, 2, 3, 4, 5};
        auto Array = Vector | Retro::Ranges::To<TArray>();
        CHECK(Array.Num() == 5);
        CHECK(Array.Max() == 5);
    }

    SECTION("Can enumerate a TArray into a TMap") {
        TArray Array = {1, 2, 3, 4, 5};
        auto Map = Array | Retro::Ranges::Views::Enumerate | Retro::Ranges::To<TMap>();

        CHECK(Map.Num() == 5);
    }

    SECTION("Can splice a string together with a character") {
        static_assert(std::ranges::contiguous_range<FStringView>);
        static_assert(std::ranges::contiguous_range<FString>);
        static_assert(std::ranges::sized_range<FString>);
        static_assert(Retro::Ranges::ReservableContainer<FString>);
        std::array Strings = {FStringView(TEXT("This")), FStringView(TEXT("is")), FStringView(TEXT("a")),
                              FStringView(TEXT("test."))};
        auto Joined = Strings | Retro::Ranges::Views::JoinWith(static_cast<TCHAR>(' ')) | Retro::Ranges::To<FString>();
        CHECK(Joined == TEXT("This is a test."));
    }

    SECTION("Can splice a string together with a string literal") {
        std::array Strings = {FStringView(TEXT("1")), FStringView(TEXT("2")), FStringView(TEXT("3")),
                              FStringView(TEXT("4"))};
        auto Constraction = FStringView(TEXT(", "));
        auto Joined = Strings | Retro::Ranges::Views::JoinWith(Constraction) | Retro::Ranges::To<FString>();
        CHECK(Joined == TEXT("1, 2, 3, 4"));
    }
}