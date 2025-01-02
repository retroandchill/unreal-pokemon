/**
 * @file PolymorphicTest.cpp
 * @brief Test for the Polymorphic class
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#include "TestAdapter.h"

#if RETROLIB_WITH_MODULES
import std;
import RetroLib;
#else
#include "RetroLib.h"

#include <array>
#include <memory>
#endif

class Base {
  public:
    virtual ~Base() = default;

    virtual int GetValue() const {
        return 0;
    }

    virtual const std::type_info &GetType() const {
        return typeid(Base);
    }
};

class Derived1 : public Base {
  public:
    explicit Derived1(int Value) : Value(Value) {
    }

    int GetValue() const override {
        return Value;
    }

    const std::type_info &GetType() const override {
        return typeid(Derived1);
    }

  private:
    int Value;
};

class Derived2 : public Base {
  public:
    explicit Derived2(const std::array<int, 15> &Values) : Values(Values) {
    }

    int GetValue() const override {
        int Value = 0;
        for (int Val : Values) {
            Value += Val;
        }
        return Value;
    }

    const std::type_info &GetType() const override {
        return typeid(Derived2);
    }

  private:
    std::array<int, 15> Values;
};

class Derived3 : public Base {
  public:
    explicit Derived3(std::shared_ptr<int> Value) : Value(std::move(Value)) {
    }

    int GetValue() const override {
        return *Value;
    }

    const std::type_info &GetType() const override {
        return typeid(Derived3);
    }

  private:
    std::shared_ptr<int> Value;
};

constexpr std::array ValueArray1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
constexpr std::array ValueArray2 = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30};

TEST_CASE_NAMED(FPolymorphicCopyTest, "RetroLib::Utils::Polymorphic::Copying", "[utils]") {
    // We want to test that we can assign different polymorphic values into each other
    Retro::TPolymorphic<Base> Polymorphic1 = Derived1(42);
    CHECK(Polymorphic1->GetValue() == 42);
    CHECK(Polymorphic1.GetSize() == sizeof(Derived1));

    Retro::TPolymorphic<Base> Polymorphic2(std::in_place_type<Derived2>, ValueArray1);
    CHECK(Polymorphic2->GetValue() == 120);
    CHECK(Polymorphic2.GetSize() == sizeof(Derived2));

    Polymorphic1 = Polymorphic2;
    CHECK(Polymorphic1->GetValue() == 120);

    Polymorphic2.Emplace<Derived1>(40);
    Polymorphic1 = Polymorphic2;
    CHECK(Polymorphic1->GetValue() == 40);

    Polymorphic1 = Retro::TPolymorphic<Base>(std::in_place_type<Derived2>, ValueArray2);
    CHECK(Polymorphic1->GetValue() == 240);

    // Here we want to check that during reassignment the destructor for a different type gets invoked
    auto Value = std::make_shared<int>(4);
    std::weak_ptr<int> WeakValue = Value;
    Polymorphic1 = Derived3(std::move(Value));
    CHECK(Polymorphic1->GetValue() == 4);

    Polymorphic1 = Retro::TPolymorphic<Base>();
    CHECK(Polymorphic1->GetValue() == 0);
    CHECK(WeakValue.expired());

    // Here we want to verify that the polymorphic values can copy into like types when using small storage
    Polymorphic1 = Derived1(12);
    Polymorphic2 = Derived1(64);
    CHECK(Polymorphic1->GetValue() == 12);
    CHECK(Polymorphic2->GetValue() == 64);

    Polymorphic1 = Polymorphic2;
    CHECK(Polymorphic1->GetValue() == 64);
    Polymorphic1 = Retro::TPolymorphic<Base>(std::in_place_type<Derived1>, 100);
    CHECK(Polymorphic1->GetValue() == 100);

    // Here we're going to do the same thing with large storage
    Polymorphic1.Emplace<Derived2>(ValueArray1);
    Polymorphic2.Emplace<Derived2>(ValueArray2);
    CHECK(Polymorphic1->GetValue() == 120);
    CHECK(Polymorphic2->GetValue() == 240);

    Polymorphic1 = Polymorphic2;
    CHECK(Polymorphic1->GetValue() == 240);
    Polymorphic1 = Retro::TPolymorphic<Base>(std::in_place_type<Derived2>, ValueArray1);
    CHECK(Polymorphic1->GetValue() == 120);

    // Check that dereferencing works correctly
    auto &Dereferenced1 = *Polymorphic1;
    CHECK(Dereferenced1.GetValue() == 120);

    const auto Polymorphic3 = Retro::TPolymorphic<Base>(std::in_place_type<Derived1>, 150);
    auto &Dereferenced2 = *Polymorphic3;
    CHECK(Dereferenced2.GetValue() == 150);

    Polymorphic1 = Retro::TPolymorphic<Base>(std::in_place_type<Derived2>, ValueArray1);
    const Retro::TPolymorphic<Base> Polymorphic4 = Polymorphic1;
    auto &Dereferenced3 = *Polymorphic4;
    CHECK(Dereferenced3.GetValue() == 120);
}

#ifdef __UNREAL__
TEST_CASE_NAMED(FPolymorphicOptionalState, "RetroLib::Utils::Polymorphic::IntrusiveOptional", "[utils]") {
    static_assert(sizeof(Retro::TPolymorphic<Base>) == sizeof(TOptional<Retro::TPolymorphic<Base>>));
    TOptional<Retro::TPolymorphic<Base>> Optional1;
    CHECK_FALSE(Optional1.IsSet());
    Optional1.Emplace(std::in_place_type<Derived1>, 12);
    REQUIRE(Optional1.IsSet());
    auto ObtainedValue1 = Optional1->Get();
    CHECK(ObtainedValue1->GetType() == typeid(Derived1));
    auto ObtainedValue2 = std::as_const(Optional1)->Get();
    CHECK(ObtainedValue2->GetType() == typeid(Derived1));

    auto ObtainedValue3 = *Optional1;
    CHECK(ObtainedValue3->GetValue() == 12);
    auto ObtainedValue4 = *std::as_const(Optional1);
    CHECK(ObtainedValue4->GetValue() == 12);

    auto ObtainedValue5 = *TOptional(Retro::TPolymorphic<Base>(std::in_place_type<Derived1>, 24));
    CHECK(ObtainedValue5->GetValue() == 24);

    auto Optional2 = Optional1;
    Optional1.Reset();
    CHECK(Optional2.IsSet());
    CHECK_FALSE(Optional1.IsSet());

    std::swap(Optional1, Optional2);
    CHECK_FALSE(Optional2.IsSet());
    CHECK(Optional1.IsSet());
}
#endif