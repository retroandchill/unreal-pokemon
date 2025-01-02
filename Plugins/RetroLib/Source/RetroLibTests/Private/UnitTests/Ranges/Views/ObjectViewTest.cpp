#include "TestAdapter.h"
#include "RetroLib.h"
#include "Slate/SlateTextureAtlasInterface.h"

TEST_CASE_NAMED(FObjectViewTest, "RetroLib::Ranges::Views::ObjectView", "[RetroLib][Ranges]") {
	SECTION("Can iterate over a view of objects") {
		static_assert(std::ranges::input_range<Retro::Ranges::TObjectView<UClass>>);
		auto ActorClasses = Retro::Ranges::TObjectView<UClass>() |
			Retro::Ranges::Views::Filter([](const UClass* Class) { return Class->IsChildOf<AActor>(); }) |
				Retro::Ranges::To<TArray>();
		CHECK(ActorClasses.Num() > 0);
		CHECK(std::ranges::all_of(ActorClasses, [](const UClass* Class) { return Class->IsChildOf<AActor>(); }));
	}
}

TEST_CASE_NAMED(FClassViewTest, "RetroLib::Ranges::Views::ClassView", "[RetroLib][Ranges]") {
	SECTION("Can iterate over a view of classes") {
		static_assert(std::ranges::input_range<Retro::Ranges::TClassView<AActor>>);
		auto ActorClasses = Retro::Ranges::TClassView<AActor>() |
				Retro::Ranges::To<TArray>();
		CHECK(ActorClasses.Num() > 0);
		CHECK(std::ranges::all_of(ActorClasses, [](const UClass* Class) { return Class->IsChildOf<AActor>(); }));
	}

	SECTION("Can iterate over a view of classes") {
		static_assert(std::ranges::input_range<Retro::Ranges::TClassView<ISlateTextureAtlasInterface>>);
		auto ActorClasses = Retro::Ranges::TClassView<ISlateTextureAtlasInterface>() |
				Retro::Ranges::To<TArray>();
		CHECK(ActorClasses.Num() > 0);
		CHECK(std::ranges::all_of(ActorClasses, [](const UClass* Class) { return Class->ImplementsInterface(USlateTextureAtlasInterface::StaticClass()); }));
	}
}