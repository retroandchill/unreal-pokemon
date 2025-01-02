#include "TestAdapter.h"
#include "RetroLib.h"
#include <array>

TEST_CASE_NAMED(FOptionalPipesTest, "RetroLib::Optionals::Pipes", "[RetroLib][Optionals]") {
	SECTION("Can pipe an TOptional object") {
		TOptional Value = 4;
		auto Result = Value |
			Retro::Optionals::Transform([](int i) { return i * 2; }) |
				Retro::Optionals::Value;
		CHECK(Result == 8);

		Value.Reset();
		Result = Value |
			Retro::Optionals::Transform([](int i) { return i * 2; }) |
				Retro::Optionals::OrElseValue(12);
		CHECK(Result == 12);
	}

	SECTION("Can pipe an std::optional into TOptional") {
		std::optional Value3 = 5;
		auto AsUeOptional3 = Value3 |
			Retro::Optionals::To<TOptional>();
		CHECK(AsUeOptional3.GetValue() == 5);

		auto Value4 = Retro::Optionals::MakeOptionalReference(Value3);
		auto AsUeOptional4 = Value4 |
			Retro::Optionals::To<TOptional>();
		CHECK(AsUeOptional4.GetValue() == 5);
	}

	SECTION("TOptional has range semantics") {
		static_assert(std::ranges::input_range<TOptional<int32>>);
		TOptional Value = 1;
		auto AsVector = Value |
			Retro::Ranges::To<std::vector>();
		REQUIRE(AsVector.size() == 1);
		CHECK(AsVector[0] == 1);

		std::array<TOptional<int32>, 10> Array = {1, {}, 3, 4, 5, {}, {}, 6, {}, 8};
		auto Joined = Array |
			std::ranges::views::join |
				Retro::Ranges::To<TArray>();
		REQUIRE(Joined.Num() == 6);
	}
}
