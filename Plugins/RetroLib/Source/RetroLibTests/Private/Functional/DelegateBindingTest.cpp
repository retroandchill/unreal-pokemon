#include "RetroLib.h"
#include "TestAdapter.h"

#include <array>

namespace Retro::Testing::Delegates {
    DECLARE_DELEGATE_RetVal_TwoParams(bool, FDemoDelegate, FStringView, int32);
    DECLARE_MULTICAST_DELEGATE_TwoParams(FDemoMulticastDelegate, FStringView, int32);
    DECLARE_DELEGATE_RetVal(FString, FGetObjectName);
    DECLARE_DELEGATE_OneParam(FAddToValue, int32)
    DECLARE_DELEGATE_OneParam(FAddToArray, TArray<int32> &);
    
    DECLARE_MULTICAST_DELEGATE_OneParam(FMultiGetObjectName, FString&);
    DECLARE_MULTICAST_DELEGATE_OneParam(FMultiAddToValue, int32)
    DECLARE_MULTICAST_DELEGATE_OneParam(FMultiAddToArray, TArray<int32> &);

    static_assert(Retro::Delegates::NativeDelegate<FDemoDelegate>);
    static_assert(Retro::Delegates::UnicastDelegate<FDemoDelegate>);
    static_assert(Retro::Delegates::UEDelegate<FDemoDelegate>);
    static_assert(Retro::Delegates::NativeDelegate<FDemoMulticastDelegate>);
    static_assert(Retro::Delegates::MulticastDelegate<FDemoMulticastDelegate>);
    static_assert(Retro::Delegates::UEDelegate<FDemoMulticastDelegate>);

    static bool IsLength(FStringView View, int32 Length) {
        return View.Len() == Length;
    }

    static void AddValue(TArray<int32> &Array, int32 Value) {
        Array.Add(Value);
    }

    class FDemoClass : public TSharedFromThis<FDemoClass> {
      public:
        explicit FDemoClass(int32 Value) : Value(Value) {
        }

        int32 GetValue() const {
            return Value;
        }

        void AddToValue(int32 Other) {
            Value += Other;
        }

      private:
        int32 Value;
    };

    class FUnsharedDemoClass {
      public:
        explicit FUnsharedDemoClass(int32 Value) : Value(Value) {
        }

        int32 GetValue() const {
            return Value;
        }

        void AddToValue(int32 Other) {
            Value += Other;
        }

      private:
        int32 Value;
    };
} // namespace retro::testing::delegates

TEST_CASE_NAMED(FDelegateBindingTest, "RetroLib::Functional::Delegates::Wrapping", "[RetroLib][Functional]") {
    using namespace Retro::Testing::Delegates;

    SECTION("Should be able to bind a regular delegate") {
        FDemoDelegate Delegate;
        auto Binding = Retro::CreateBinding(FDemoDelegate::CreateStatic(&IsLength), 10);
        CHECK(Binding(TEXT("1234567890")));
        CHECK_FALSE(Binding(TEXT("123456789")));
    }

    SECTION("Should be able to bind a multicast delegate") {
        FDemoMulticastDelegate Delegate;
        int32 Findings = 0;
        Delegate.Add(FDemoMulticastDelegate::FDelegate::CreateLambda([&Findings](FStringView View, int32 Length) {
            if (View.Len() == Length) {
                Findings++;
            }
        }));

        auto Binding = Retro::CreateBinding(Delegate, 10);
        Binding(TEXT("1234567890"));
        Binding(TEXT("123456789"));
        CHECK(Findings == 1);
    }

    SECTION("Can binding a delegate to a range pipe") {
        std::array Strings = {FStringView(TEXT("1234567890")), FStringView(TEXT("123456789"))};

        auto ValidStrings = Strings | Retro::Ranges::Views::Filter(FDemoDelegate::CreateStatic(&IsLength), 10) |
                            Retro::Ranges::To<TArray>();

        REQUIRE(ValidStrings.Num() == 1);
        CHECK(ValidStrings[0] == TEXT("1234567890"));
    }
}

TEST_CASE_NAMED(FCreateDelegateTest, "RetroLib::Functional::Delegates::Creation", "[RetroLib][Functional]") {
    using namespace Retro::Testing::Delegates;

    SECTION("Can bind free functions and lambdas") {
        static_assert(Retro::Delegates::CanBindStatic<FAddToArray, decltype(&AddValue), int32>);
        static_assert(Retro::Delegates::CanBindLambda<FAddToArray, decltype(&AddValue), int32>);
        TArray<int32> Array;
        auto Delegate1 = Retro::Delegates::Create<FAddToArray>(&AddValue, 4);
        Delegate1.Execute(Array);
        REQUIRE(Array.Num() == 1);
        CHECK(Array[0] == 4);

        int Value = 12;
        auto Lambda = [Value](TArray<int32> &A) { A.Add(Value); };
        static_assert(!Retro::Delegates::CanBindStatic<FAddToArray, decltype(Lambda)>);
        static_assert(Retro::Delegates::CanBindLambda<FAddToArray, decltype(Lambda)>);
        auto Delegate2 = Retro::Delegates::Create<FAddToArray>(Lambda);
        Delegate2.Execute(Array);
        REQUIRE(Array.Num() == 2);
        CHECK(Array[1] == 12);
    }

    SECTION("Can bind UObject members") {
        auto Object = NewObject<UDataTable>();
        static_assert(Retro::Delegates::CanBindUObject<FGetObjectName, UObject *, FString (UObject::*)() const>);
        static_assert(!Retro::Delegates::CanBindWeakLambda<FGetObjectName, UObject *, FString (UObject::*)() const>);
        static_assert(!Retro::Delegates::CanBindSP<FGetObjectName, UObject *, FString (UObject::*)() const>);
        static_assert(!Retro::Delegates::CanBindSPLambda<FGetObjectName, UObject *, FString (UObject::*)() const>);
        static_assert(Retro::Delegates::CanBindRaw<FGetObjectName, UObject *, FString (UObject::*)() const>);
        auto Delegate1 =
            Retro::Delegates::Create<FGetObjectName>(Object, static_cast<FString (UObject::*)() const>(&UObject::GetName));
        CHECK(Delegate1.Execute() == Object->GetName());

        int Value = 12;
        auto Lambda = [&Value] { Value *= 2; };
        static_assert(Retro::Delegates::CanBindWeakLambda<FSimpleDelegate, UObject *, decltype(Lambda)>);
        static_assert(!Retro::Delegates::CanBindSPLambda<FSimpleDelegate, UObject *, decltype(Lambda)>);
        auto Delegate2 = Retro::Delegates::Create<FSimpleDelegate>(Lambda);
        Delegate2.Execute();
        CHECK(Value == 24);
    }

    SECTION("Can bind to shared pointers") {
        auto SharedValue = MakeShared<FDemoClass>(12);
        static_assert(!Retro::Delegates::CanBindUObject<FGetObjectName, TSharedRef<FDemoClass> &, FString (UObject::*)() const>);
        static_assert(!Retro::Delegates::CanBindUObject<FGetObjectName, FDemoClass *, FString (UObject::*)() const>);
        static_assert(Retro::Delegates::CanBindSP<FAddToValue, TSharedRef<FDemoClass> &, decltype(&FDemoClass::AddToValue)>);
        static_assert(Retro::Delegates::CanBindSP<FAddToValue, FDemoClass *, decltype(&FDemoClass::AddToValue)>);

        auto Delegate1 = Retro::Delegates::Create<FAddToValue>(SharedValue, &FDemoClass::AddToValue);
        Delegate1.Execute(10);
        CHECK(SharedValue->GetValue() == 22);

        auto Delegate2 = Retro::Delegates::Create<FAddToValue>(&SharedValue.Get(), &FDemoClass::AddToValue);
        Delegate2.Execute(13);
        CHECK(SharedValue->GetValue() == 35);

        auto Lambda = [&RawValue = SharedValue.Get()](int32 Value) { RawValue.AddToValue(Value); };
        static_assert(Retro::Delegates::CanBindSPLambda<FAddToValue, TSharedRef<FDemoClass> &, decltype(Lambda)>);
        static_assert(Retro::Delegates::CanBindSPLambda<FAddToValue, FDemoClass *, decltype(Lambda)>);
        auto Delegate3 = Retro::Delegates::Create<FAddToValue>(SharedValue, Lambda);
        Delegate3.Execute(15);
        CHECK(SharedValue->GetValue() == 50);
    }

    SECTION("Can bind to raw pointers") {
        FUnsharedDemoClass RawValue(12);

        static_assert(Retro::Delegates::CanBindRaw<FAddToValue, FUnsharedDemoClass *, decltype(&FUnsharedDemoClass::AddToValue)>);
        static_assert(
            !Retro::Delegates::CanBindUObject<FAddToValue, FUnsharedDemoClass *, decltype(&FUnsharedDemoClass::AddToValue)>);
        static_assert(!Retro::Delegates::CanBindSP<FAddToValue, FUnsharedDemoClass *, decltype(&FDemoClass::AddToValue)>);

        auto Delegate1 = Retro::Delegates::Create<FAddToValue>(&RawValue, &FUnsharedDemoClass::AddToValue);
        Delegate1.Execute(10);
        CHECK(RawValue.GetValue() == 22);
    }
}

TEST_CASE_NAMED(FBindDelegateTest, "RetroLib::Functional::Delegates::Binding", "[RetroLib][Functional]") {
    using namespace Retro::Testing::Delegates;

    SECTION("Can bind free functions and lambdas") {
        static_assert(Retro::Delegates::CanBindStatic<FAddToArray, decltype(&AddValue), int32>);
        static_assert(Retro::Delegates::CanBindLambda<FAddToArray, decltype(&AddValue), int32>);
        TArray<int32> Array;
        FAddToArray Delegate1;
        Retro::Delegates::Bind(Delegate1, &AddValue, 4);
        Delegate1.Execute(Array);
        REQUIRE(Array.Num() == 1);
        CHECK(Array[0] == 4);

        int Value = 12;
        auto Lambda = [Value](TArray<int32> &A) { A.Add(Value); };
        static_assert(!Retro::Delegates::CanBindStatic<FAddToArray, decltype(Lambda)>);
        static_assert(Retro::Delegates::CanBindLambda<FAddToArray, decltype(Lambda)>);
        FAddToArray Delegate2;
        Delegate2 | Retro::Delegates::Bind(Lambda);
        Delegate2.Execute(Array);
        REQUIRE(Array.Num() == 2);
        CHECK(Array[1] == 12);
    }

    SECTION("Can bind UObject members") {
        auto Object = NewObject<UDataTable>();
        static_assert(Retro::Delegates::CanBindUObject<FGetObjectName, UObject *, FString (UObject::*)() const>);
        static_assert(!Retro::Delegates::CanBindWeakLambda<FGetObjectName, UObject *, FString (UObject::*)() const>);
        static_assert(!Retro::Delegates::CanBindSP<FGetObjectName, UObject *, FString (UObject::*)() const>);
        static_assert(!Retro::Delegates::CanBindSPLambda<FGetObjectName, UObject *, FString (UObject::*)() const>);
        static_assert(Retro::Delegates::CanBindRaw<FGetObjectName, UObject *, FString (UObject::*)() const>);
        FGetObjectName Delegate1;
        Retro::Delegates::Bind(Delegate1, Object, static_cast<FString (UObject::*)() const>(&UObject::GetName));
        CHECK(Delegate1.Execute() == Object->GetName());

        int Value = 12;
        auto Lambda = [&Value] { Value *= 2; };
        static_assert(Retro::Delegates::CanBindWeakLambda<FSimpleDelegate, UObject *, decltype(Lambda)>);
        static_assert(!Retro::Delegates::CanBindSPLambda<FSimpleDelegate, UObject *, decltype(Lambda)>);
        FSimpleDelegate Delegate2;
        Retro::Delegates::Bind(Delegate2, Lambda);
        Delegate2.Execute();
        CHECK(Value == 24);
    }

    SECTION("Can bind to shared pointers") {
        auto SharedValue = MakeShared<FDemoClass>(12);
        static_assert(!Retro::Delegates::CanBindUObject<FGetObjectName, TSharedRef<FDemoClass> &, FString (UObject::*)() const>);
        static_assert(!Retro::Delegates::CanBindUObject<FGetObjectName, FDemoClass *, FString (UObject::*)() const>);
        static_assert(Retro::Delegates::CanBindSP<FAddToValue, TSharedRef<FDemoClass> &, decltype(&FDemoClass::AddToValue)>);
        static_assert(Retro::Delegates::CanBindSP<FAddToValue, FDemoClass *, decltype(&FDemoClass::AddToValue)>);

        FAddToValue Delegate1;
        Retro::Delegates::Bind(Delegate1, SharedValue, &FDemoClass::AddToValue);
        Delegate1.Execute(10);
        CHECK(SharedValue->GetValue() == 22);

        FAddToValue Delegate2;
        Retro::Delegates::Bind(Delegate2, &SharedValue.Get(), &FDemoClass::AddToValue);
        Delegate2.Execute(13);
        CHECK(SharedValue->GetValue() == 35);

        auto Lambda = [&RawValue = SharedValue.Get()](int32 Value) { RawValue.AddToValue(Value); };
        static_assert(Retro::Delegates::CanBindSPLambda<FAddToValue, TSharedRef<FDemoClass> &, decltype(Lambda)>);
        static_assert(Retro::Delegates::CanBindSPLambda<FAddToValue, FDemoClass *, decltype(Lambda)>);
        FAddToValue Delegate3;
        Retro::Delegates::Bind(Delegate3, SharedValue, Lambda);
        Delegate3.Execute(15);
        CHECK(SharedValue->GetValue() == 50);
    }

    SECTION("Can bind to raw pointers") {
        FUnsharedDemoClass RawValue(12);

        static_assert(Retro::Delegates::CanBindRaw<FAddToValue, FUnsharedDemoClass *, decltype(&FUnsharedDemoClass::AddToValue)>);
        static_assert(
            !Retro::Delegates::CanBindUObject<FAddToValue, FUnsharedDemoClass *, decltype(&FUnsharedDemoClass::AddToValue)>);
        static_assert(!Retro::Delegates::CanBindSP<FAddToValue, FUnsharedDemoClass *, decltype(&FDemoClass::AddToValue)>);

        FAddToValue Delegate1;
        Retro::Delegates::Bind(Delegate1, &RawValue, &FUnsharedDemoClass::AddToValue);
        Delegate1.Execute(10);
        CHECK(RawValue.GetValue() == 22);
    }
}

TEST_CASE_NAMED(FAddDelegateTest, "RetroLib::Functional::Delegates::Adding", "[RetroLib][Functional]") {
    using namespace Retro::Testing::Delegates;

    SECTION("Can bind free functions and lambdas") {
        static_assert(Retro::Delegates::CanAddStatic<FMultiAddToArray, decltype(&AddValue), int32>);
        static_assert(Retro::Delegates::CanAddLambda<FMultiAddToArray, decltype(&AddValue), int32>);
        TArray<int32> Array;
        FMultiAddToArray Delegate1;
        Retro::Delegates::Add(Delegate1, &AddValue, 4);
        Delegate1.Broadcast(Array);
        REQUIRE(Array.Num() == 1);
        CHECK(Array[0] == 4);

        int Value = 12;
        auto Lambda = [Value](TArray<int32> &A) { A.Add(Value); };
        static_assert(!Retro::Delegates::CanAddStatic<FMultiAddToArray, decltype(Lambda)>);
        static_assert(Retro::Delegates::CanAddLambda<FMultiAddToArray, decltype(Lambda)>);
        FMultiAddToArray Delegate2;
        Delegate2 | Retro::Delegates::Add(Lambda);
        Delegate2.Broadcast(Array);
        REQUIRE(Array.Num() == 2);
        CHECK(Array[1] == 12);
    }

    SECTION("Can bind UObject members") {
        auto Object = NewObject<UDataTable>();
        static_assert(Retro::Delegates::CanAddUObject<FMultiGetObjectName, UObject *, void (UObject::*)(FString&) const>);
        static_assert(!Retro::Delegates::CanAddWeakLambda<FMultiGetObjectName, UObject *, void (UObject::*)(FString&) const>);
        static_assert(!Retro::Delegates::CanAddSP<FMultiGetObjectName, UObject *, void (UObject::*)(FString&) const>);
        static_assert(!Retro::Delegates::CanAddSPLambda<FMultiGetObjectName, UObject *,void (UObject::*)(FString&) const>);
        static_assert(Retro::Delegates::CanAddRaw<FMultiGetObjectName, UObject *, void (UObject::*)(FString&) const>);
        FMultiGetObjectName Delegate1;
        Retro::Delegates::Add(Delegate1, Object, static_cast<void (UObject::*)(FString&) const>(&UObject::GetName));
        FString Name;
        Delegate1.Broadcast(Name);
        CHECK(Name == Object->GetName());

        int Value = 12;
        auto Lambda = [&Value] { Value *= 2; };
        static_assert(Retro::Delegates::CanAddWeakLambda<FSimpleMulticastDelegate, UObject *, decltype(Lambda)>);
        static_assert(!Retro::Delegates::CanAddSPLambda<FSimpleMulticastDelegate, UObject *, decltype(Lambda)>);
        FSimpleMulticastDelegate Delegate2;
        Retro::Delegates::Add(Delegate2, Lambda);
        Delegate2.Broadcast();
        CHECK(Value == 24);
    }

    SECTION("Can bind to shared pointers") {
        auto SharedValue = MakeShared<FDemoClass>(12);
        static_assert(Retro::Delegates::CanAddSP<FMultiAddToValue, TSharedRef<FDemoClass> &, decltype(&FDemoClass::AddToValue)>);
        static_assert(Retro::Delegates::CanAddSP<FMultiAddToValue, FDemoClass *, decltype(&FDemoClass::AddToValue)>);

        FMultiAddToValue Delegate1;
        Retro::Delegates::Add(Delegate1, SharedValue, &FDemoClass::AddToValue);
        Delegate1.Broadcast(10);
        CHECK(SharedValue->GetValue() == 22);

        FMultiAddToValue Delegate2;
        Retro::Delegates::Add(Delegate2, &SharedValue.Get(), &FDemoClass::AddToValue);
        Delegate2.Broadcast(13);
        CHECK(SharedValue->GetValue() == 35);

        auto Lambda = [&RawValue = SharedValue.Get()](int32 Value) { RawValue.AddToValue(Value); };
        static_assert(Retro::Delegates::CanAddSPLambda<FMultiAddToValue, FDemoClass *, decltype(Lambda)>);
        FMultiAddToValue Delegate3;
        Retro::Delegates::Add(Delegate3, &SharedValue.Get(), Lambda);
        Delegate3.Broadcast(15);
        CHECK(SharedValue->GetValue() == 50);
    }

    SECTION("Can bind to raw pointers") {
        FUnsharedDemoClass RawValue(12);

        static_assert(Retro::Delegates::CanAddRaw<FMultiAddToValue, FUnsharedDemoClass *, decltype(&FUnsharedDemoClass::AddToValue)>);
        static_assert(
            !Retro::Delegates::CanAddUObject<FMultiAddToValue, FUnsharedDemoClass *, decltype(&FUnsharedDemoClass::AddToValue)>);
        static_assert(!Retro::Delegates::CanAddSP<FMultiAddToValue, FUnsharedDemoClass *, decltype(&FDemoClass::AddToValue)>);

        FMultiAddToValue Delegate1;
        Retro::Delegates::Add(Delegate1, &RawValue, &FUnsharedDemoClass::AddToValue);
        Delegate1.Broadcast(10);
        CHECK(RawValue.GetValue() == 22);
    }
}