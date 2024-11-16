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
        
    });
}