#include "TestAdapter.h"
#include "Misc/AutomationTest.h"
#include "RetroLib.h"

#include "Utilities/TestStructs.h"

BEGIN_DEFINE_SPEC(FTestOpaqueStruct, "Unit Tests.Ranges.Structs",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);

END_DEFINE_SPEC(FTestOpaqueStruct);

void FTestOpaqueStruct::Define() {
    Describe("Test Opaque Struct Type", [this] {
        BeforeEach([] {});

        It("A default constructed opaque struct should be empty.", [this] {
            Retro::FOpaqueStruct Struct;
            CHECK_FALSE(Struct.HasValue());
            return true;
        });

        It("An opaque struct created from a struct reference is default constructed", [this] {
            const Retro::FOpaqueStruct Struct(*FTestStruct::StaticStruct());
            REQUIRE(Struct.IsStruct<FTestStruct>());
            auto &Underlying = Struct.Get<FTestStruct>();
            CHECK(Underlying.Value1 == 3);
        });

        It("Copying an opaque struct maintains the existing value", [this] {
            Retro::FOpaqueStruct Struct1(std::in_place_type<FTestStruct>);
            REQUIRE(Struct1.IsStruct<FTestStruct>());
            auto &Underlying = Struct1.Get<FTestStruct>();
            auto WeakPtr = Underlying.Value3.ToWeakPtr();
            Retro::FOpaqueStruct Struct2 = Struct1;
            REQUIRE(Struct1.IsStruct<FTestStruct>());
            auto &Underlying2 = Struct2.Get<FTestStruct>();
            Underlying2.Value3.Reset();
            REQUIRE(WeakPtr.IsValid());
        });

        It("Moving an opaque struct migrates the data", [this] {
            Retro::FOpaqueStruct Struct1(std::in_place_type<FTestStruct>);
            REQUIRE(Struct1.IsStruct<FTestStruct>());
            auto &Underlying = Struct1.Get<FTestStruct>();
            auto WeakPtr = Underlying.Value3.ToWeakPtr();
            Retro::FOpaqueStruct Struct2 = std::move(Struct1);
            REQUIRE(Struct2.IsStruct<FTestStruct>());
            auto &Underlying2 = Struct2.Get<FTestStruct>();
            Underlying2.Value3.Reset();
            REQUIRE(!WeakPtr.IsValid());
        });

        It("Reassigning a struct is possible", [this] {
            Retro::FOpaqueStruct Struct1(FTransform{});
            REQUIRE(Struct1.IsStruct<FTransform>());

            // Create a new struct and copy the old value into it
            Retro::FOpaqueStruct Struct2(std::in_place_type<FTestStruct>);
            Struct1 = Struct2;
            REQUIRE(Struct1.IsStruct<FTestStruct>());

            FTransform Transform;
            Struct1 = Retro::FOpaqueStruct(*Retro::GetScriptStruct<FTransform>(), &Transform);
            REQUIRE(Struct1.IsStruct<FTransform>());

            Struct2.Emplace<FTransform>();
            Struct1 = Struct2;
            REQUIRE(Struct1.IsStruct<FTransform>());
        });

        It("Can directly set a value into the struct", [this] {
            Retro::FOpaqueStruct Struct;
            Struct = FTransform{};
            CHECK(Struct.IsStruct<FTransform>());
            CHECK(Struct.TryGet<FTransform>().IsSet());
            CHECK(Struct.TryGetRaw().IsSet());
            CHECK(const_cast<const Retro::FOpaqueStruct &>(Struct).TryGetRaw().IsSet());

            Struct = FTestStruct();
            CHECK(Struct.IsStruct<FTestStruct>());
            CHECK_FALSE(Struct.TryGet<FTransform>().IsSet());

            Struct.Emplace(*Retro::GetScriptStruct<FTransform>());
            CHECK(Struct.IsStruct<FTransform>());
            CHECK(const_cast<const Retro::FOpaqueStruct &>(Struct).TryGet<FTransform>().IsSet());

            Struct.Emplace(*Retro::GetScriptStruct<FTestStruct>());
            CHECK(Struct.IsStruct<FTestStruct>());
            CHECK_FALSE(const_cast<const Retro::FOpaqueStruct &>(Struct).TryGet<FTransform>().IsSet());

            FTransform Transform;
            Struct.Emplace(*Retro::GetScriptStruct<FTransform>(), &Transform);
            CHECK(Struct.IsStruct<FTransform>());
            bool bIdentical = false;
            CHECK(Retro::GetScriptStruct<FTransform>()->GetCppStructOps()->Identical(
                Struct.GetRaw(), &Transform, PPF_None, bIdentical));
            CHECK(bIdentical);

            FVector Vector;
            Struct.Emplace(*Retro::GetScriptStruct<FVector>(), &Vector);
            CHECK(Struct.IsStruct<FVector>());
            REQUIRE(Retro::GetScriptStruct<FVector>()->GetCppStructOps()->Identical(
                const_cast<const Retro::FOpaqueStruct &>(Struct).GetRaw(), &Vector, PPF_None, bIdentical));
            CHECK(bIdentical);

            Struct.Emplace<FTransform>();
            CHECK(Struct.IsStruct<FTransform>());

            Struct.Emplace<FRotator>();
            CHECK(Struct.IsStruct<FRotator>());
        });
    });
}