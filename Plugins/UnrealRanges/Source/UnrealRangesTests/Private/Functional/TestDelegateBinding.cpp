#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Ranges/Functional/Bindings.h"

DECLARE_DELEGATE_RetVal_TwoParams(bool, FDemoDelegate, FStringView, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FDemoMulticastDelegate, FStringView, int32);

static_assert(UE::Ranges::NativeDelegate<FDemoDelegate>);
static_assert(UE::Ranges::UnicastDelegate<FDemoDelegate>);
static_assert(UE::Ranges::UEDelegate<FDemoDelegate>);
static_assert(UE::Ranges::NativeDelegate<FDemoMulticastDelegate>);
static_assert(UE::Ranges::MulticastDelegate<FDemoMulticastDelegate>);
static_assert(UE::Ranges::UEDelegate<FDemoMulticastDelegate>);

BEGIN_DEFINE_SPEC(FTestDelegateBinding, "Unit Tests.Ranges.Functional",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);

static bool IsLength(FStringView View, int32 Length);

END_DEFINE_SPEC(FTestDelegateBinding);

void FTestDelegateBinding::Define() {
    Describe("Test Delegate Binding", [this] {
        It("Should be able to bind a regular delegate", [this] {
            auto Binding = UE::Ranges::CreateBinding(FDemoDelegate::CreateStatic(&FTestDelegateBinding::IsLength), 10);
            UE_CHECK_TRUE(Binding(TEXT("1234567890")));
            UE_CHECK_FALSE(Binding(TEXT("123456789")));
            return true;
        });

        It("Should be able to bind a multicast delegate", [this] {
            FDemoMulticastDelegate Delegate;
            int32 Findings = 0;
            Delegate.Add(FDemoMulticastDelegate::FDelegate::CreateLambda([&Findings](FStringView View, int32 Length) {
                if (View.Len() == Length) {
                    Findings++;
                }
            }));

            &FDemoMulticastDelegate::Broadcast;
            auto Binding = UE::Ranges::CreateBinding(Delegate, 10);
            Binding(TEXT("1234567890"));
            Binding(TEXT("123456789"));
            UE_CHECK_EQUAL(1, Findings);
            return true;
        });
    });
}

bool FTestDelegateBinding::IsLength(FStringView View, int32 Length) {
    return View.Len() == Length;
}
