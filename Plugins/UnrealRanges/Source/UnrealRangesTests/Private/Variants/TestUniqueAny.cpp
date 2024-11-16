#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Ranges/Variants/UniqueAny.h"

BEGIN_DEFINE_SPEC(FTestUniqueAny, "Unit Tests.Ranges.Variants",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);

END_DEFINE_SPEC(FTestUniqueAny);

void FTestUniqueAny::Define() {
    Describe("Test FUniqueAny Operations", [this] {
        BeforeEach([] {});

        It("Can move a value from one location to another", [this] {
            auto SharedPtr = MakeShared<int32>(1).ToSharedPtr();
            auto WeakPtr = SharedPtr.ToWeakPtr();
            UE::Ranges::FUniqueAny Any1(std::move(SharedPtr));

            // Move the any into another object and then reset it to void the value
            UE::Ranges::FUniqueAny Any2(std::move(Any1));
            Any2.Reset();

            UE_CHECK_FALSE(WeakPtr.IsValid());
            return true;
        });

        It("Can hold any type and change type as needed", [this] {
           UE::Ranges::FUniqueAny Any;
            UE_CHECK_FALSE(Any.HasValue());

            Any = 4;
            UE_CHECK_TRUE(Any.HasValue());
            UE_ASSERT_TRUE(Any.GetType() == typeid(int32));
            UE_CHECK_EQUAL(4, Any.Get<int32>());
            UE_CHECK_EQUAL(const_cast<const UE::Ranges::FUniqueAny&>(Any).Get<int32>(), Any.Get<int32>());

            Any.Emplace<FString>(TEXT("Hello World"));
            auto StringValue = Any.TryGet<FString>();
            UE_ASSERT_TRUE(StringValue.IsSet());
            UE_CHECK_EQUAL(TEXT("Hello World"), *StringValue);

            auto StdStringValue = const_cast<const UE::Ranges::FUniqueAny&>(Any).TryGet<std::string>();
            UE_CHECK_FALSE(StdStringValue.IsSet());
            
            Any.Emplace<std::array<FString, 10>>();
            UE_CHECK_TRUE(Any.HasValue());

            return true;
        });
    });
}