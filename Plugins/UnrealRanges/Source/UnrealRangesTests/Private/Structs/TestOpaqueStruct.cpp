#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Ranges/Structs/OpaqueStruct.h"
#include "Utilities/TestStructs.h"

BEGIN_DEFINE_SPEC(FTestOpaqueStruct, "Unit Tests.Ranges.Structs",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);

END_DEFINE_SPEC(FTestOpaqueStruct);

void FTestOpaqueStruct::Define() {
    Describe("Test Opaque Struct Type", [this] {
        BeforeEach([] {});

        It("A default constructed opaque struct should be empty.", [this] {
            UE::Ranges::FOpaqueStruct Struct;
            UE_ASSERT_FALSE(Struct.HasValue());
            return true;
        });

        It("An opaque struct created from a struct reference is default constructed", [this] {
            const UE::Ranges::FOpaqueStruct Struct(*FTestStruct::StaticStruct());
            UE_ASSERT_TRUE(Struct.IsStruct<FTestStruct>());
            auto &Underlying = Struct.Get<FTestStruct>();
            UE_CHECK_EQUAL(3, Underlying.Value1);
            return true;
        });

        It("Copying an opaque struct maintains the existing value", [this] {
            UE::Ranges::FOpaqueStruct Struct1(std::in_place_type<FTestStruct>);
            UE_ASSERT_TRUE(Struct1.IsStruct<FTestStruct>());
            auto &Underlying = Struct1.Get<FTestStruct>();
            auto WeakPtr = Underlying.Value3.ToWeakPtr();
            UE::Ranges::FOpaqueStruct Struct2 = Struct1;
            UE_ASSERT_TRUE(Struct1.IsStruct<FTestStruct>());
            auto &Underlying2 = Struct2.Get<FTestStruct>();
            Underlying2.Value3.Reset();
            UE_ASSERT_TRUE(WeakPtr.IsValid());
            return true;
        });

        It("Moving an opaque struct migrates the data", [this] {
            UE::Ranges::FOpaqueStruct Struct1(std::in_place_type<FTestStruct>);
            UE_ASSERT_TRUE(Struct1.IsStruct<FTestStruct>());
            auto &Underlying = Struct1.Get<FTestStruct>();
            auto WeakPtr = Underlying.Value3.ToWeakPtr();
            UE::Ranges::FOpaqueStruct Struct2 = std::move(Struct1);
            UE_ASSERT_TRUE(Struct2.IsStruct<FTestStruct>());
            auto &Underlying2 = Struct2.Get<FTestStruct>();
            Underlying2.Value3.Reset();
            UE_ASSERT_FALSE(WeakPtr.IsValid());
            return true;
        });

        It("Reassigning a struct is possible", [this] {
            UE::Ranges::FOpaqueStruct Struct1(FTransform{});
            UE_ASSERT_TRUE(Struct1.IsStruct<FTransform>());

            // Create a new struct and copy the old value into it
            UE::Ranges::FOpaqueStruct Struct2(std::in_place_type<FTestStruct>);
            Struct1 = Struct2;
            UE_ASSERT_TRUE(Struct1.IsStruct<FTestStruct>());

            FTransform Transform;
            Struct1 = UE::Ranges::FOpaqueStruct(*UE::Ranges::GetScriptStruct<FTransform>(), &Transform);
            UE_ASSERT_TRUE(Struct1.IsStruct<FTransform>());

            Struct2.Emplace<FTransform>();
            Struct1 = Struct2;
            UE_ASSERT_TRUE(Struct1.IsStruct<FTransform>());
            
            return true;
        });

        It("Can directly set a value into the struct", [this] {
            UE::Ranges::FOpaqueStruct Struct;
            Struct = FTransform{};
            UE_CHECK_TRUE(Struct.IsStruct<FTransform>());
            UE_CHECK_TRUE(Struct.TryGet<FTransform>().IsSet());
            UE_CHECK_TRUE(Struct.TryGetRaw().IsSet());
            UE_CHECK_TRUE(const_cast<const UE::Ranges::FOpaqueStruct&>(Struct).TryGetRaw().IsSet());
            
            Struct = FTestStruct();
            UE_CHECK_TRUE(Struct.IsStruct<FTestStruct>());
            UE_CHECK_FALSE(Struct.TryGet<FTransform>().IsSet());

            Struct.Emplace(*UE::Ranges::GetScriptStruct<FTransform>());
            UE_CHECK_TRUE(Struct.IsStruct<FTransform>());
            UE_CHECK_TRUE(const_cast<const UE::Ranges::FOpaqueStruct&>(Struct).TryGet<FTransform>().IsSet());

            Struct.Emplace(*UE::Ranges::GetScriptStruct<FTestStruct>());
            UE_CHECK_TRUE(Struct.IsStruct<FTestStruct>());
            UE_CHECK_FALSE(const_cast<const UE::Ranges::FOpaqueStruct&>(Struct).TryGet<FTransform>().IsSet());

            FTransform Transform;
            Struct.Emplace(*UE::Ranges::GetScriptStruct<FTransform>(), &Transform);
            UE_ASSERT_TRUE(Struct.IsStruct<FTransform>());
            bool bIdentical = false;
            UE_ASSERT_TRUE(UE::Ranges::GetScriptStruct<FTransform>()->GetCppStructOps()->Identical(Struct.GetRaw(), &Transform, PPF_None, bIdentical));
            UE_CHECK_TRUE(bIdentical);

            FVector Vector;
            Struct.Emplace(*UE::Ranges::GetScriptStruct<FVector>(), &Vector);
            UE_CHECK_TRUE(Struct.IsStruct<FVector>());
            UE_ASSERT_TRUE(UE::Ranges::GetScriptStruct<FVector>()->GetCppStructOps()->Identical(const_cast<const UE::Ranges::FOpaqueStruct&>(Struct).GetRaw(), &Vector, PPF_None, bIdentical));
            UE_CHECK_TRUE(bIdentical);

            Struct.Emplace<FTransform>();
            UE_CHECK_TRUE(Struct.IsStruct<FTransform>());

            Struct.Emplace<FRotator>();
            UE_CHECK_TRUE(Struct.IsStruct<FRotator>());
            
            return true;
        });
        
    });
}