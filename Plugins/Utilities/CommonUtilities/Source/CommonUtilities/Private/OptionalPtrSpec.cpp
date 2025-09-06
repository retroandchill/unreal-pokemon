#include "OptionalPtrSpec.h"
#include "Misc/AutomationTest.h"
#include "OptionalPtr.h"
#include <chrono>
#include <utility>

BEGIN_DEFINE_SPEC(FOptionalPtrSpec, "OptionalPtr.Unit",
                  EAutomationTestFlags::ProductFilter | EAutomationTestFlags::EditorContext)
MockObject *m_wrapped_obj = nullptr;
MockObject *m_default_obj = nullptr;

template <typename ResultType, typename FuncObjectType, typename... Args>
    requires std::is_member_function_pointer_v<FuncObjectType>
ResultType GetResult(FuncObjectType &&func, Args &&...args)
{
    return (m_wrapped_obj->*func)(std::forward<Args>(args)...);
}

template <typename ResultType, typename FuncObjectType>
    requires std::is_member_object_pointer_v<FuncObjectType>
ResultType GetResult(FuncObjectType &&field)
{
    return m_wrapped_obj->*field;
}

template <typename MockType, typename FuncObjectType>
void IfPresentTest(FuncObjectType &&func)
{
    bool executed = false;
    TOptionalPtr<MockType>((MockType *)m_wrapped_obj).IfPresent(std::forward<FuncObjectType>(func), executed);
    TestTrue("", m_wrapped_obj != nullptr ? executed : !executed);
}

template <typename MockType, typename DefaultType>
void OrElseTest()
{
    auto testing_obj = TOptionalPtr<MockType>((MockType *)m_wrapped_obj).OrElse((DefaultType *)m_default_obj);
    TestTrue("", testing_obj.IsSet());
    TestTrue("", std::is_same<decltype(testing_obj), TOptionalPtr<DefaultType>>::value);
    TestEqual<MockObject *>("", testing_obj.Get(), m_wrapped_obj != nullptr ? m_wrapped_obj : m_default_obj);
}

template <typename ResultType, typename MockType, typename FuncObjectType, typename... Args>
void MapTest(FuncObjectType &&func, Args &&...args)
{
    auto testing_obj = TOptionalPtr<MockType>((MockType *)(m_wrapped_obj))
                           .Map(std::forward<FuncObjectType>(func), std::forward<Args>(args)...);
    TestTrue("", m_wrapped_obj != nullptr ? testing_obj.IsSet() : !testing_obj.IsSet());
    TestTrue("", std::is_same<decltype(testing_obj), ResultType>::value);
    if (m_wrapped_obj)
    {
        TestTrue("", *testing_obj.Get() == *GetResult<decltype(testing_obj.Get())>(std::forward<FuncObjectType>(func),
                                                                                   std::forward<Args>(args)...));
    }
}

template <typename ResultType, typename MockType, typename FuncObjectType, typename... Args>
void MapToValueTest(FuncObjectType &&func, Args &&...args)
{
    auto default_obj = new std::remove_pointer_t<ResultType>(MethodEnum::Default);
    auto testing_obj = TOptionalPtr<MockType>((MockType *)m_wrapped_obj)
                           .MapToValue(default_obj, std::forward<FuncObjectType>(func), std::forward<Args>(args)...);

    ResultType result = default_obj;
    if (m_wrapped_obj != nullptr)
    {
        result = GetResult<ResultType>(std::forward<FuncObjectType>(func), std::forward<Args>(args)...);
    }

    TestEqual("", *testing_obj, *result);
    TestTrue("", std::is_same<decltype(testing_obj), ResultType>::value);

    delete default_obj;
}

template <typename ResultType, typename MockType, typename FuncObjectType, typename... Args>
void MapStaticTest(FuncObjectType &&func, Args &&...args)
{
    auto testing_obj = TOptionalPtr<MockType>((MockType *)(m_wrapped_obj))
                           .Map(std::forward<FuncObjectType>(func), std::forward<Args>(args)...);
    TestTrue("", m_wrapped_obj != nullptr ? testing_obj.IsSet() : !testing_obj.IsSet());
    TestTrue("", std::is_same<decltype(testing_obj), ResultType>::value);
}
END_DEFINE_SPEC(FOptionalPtrSpec)

void FOptionalPtrSpec::Define()
{
    Describe("Map", [this]() {
        Describe("when given a wrapped UObject pointer", [this]() {
            Describe("when valid", [this]() {
                BeforeEach([this]() { m_wrapped_obj = NewObject<UMockUObject>(); });
                AfterEach([this]() { m_wrapped_obj->Destroy(); });
                It("should map a non-const method and return set optional",
                   [this]() { MapTest<TOptionalPtr<SimpleObject>, UMockUObject>(&MockObject::Method); });
                It("should map a const method and return set optional",
                   [this]() { MapTest<TOptionalPtr<SimpleObject>, UMockUObject>(&MockObject::MethodConst); });
                It("should map a method returning const data and return set optional",
                   [this]() { MapTest<TOptionalPtr<const SimpleObject>, UMockUObject>(&MockObject::ConstMethod); });
                It("should map a const method returning const data and return set optional", [this]() {
                    MapTest<TOptionalPtr<const SimpleObject>, UMockUObject>(&MockObject::ConstMethodConst);
                });
                It("should map a method returning const pointer and return set optional",
                   [this]() { MapTest<TOptionalPtr<SimpleObject>, UMockUObject>(&MockObject::ConstPointerMethod); });
                It("should map a const method returning const pointer and return set optional", [this]() {
                    MapTest<TOptionalPtr<SimpleObject>, UMockUObject>(&MockObject::ConstPointerMethodConst);
                });
                It("should map a non-const field and return set optional",
                   [this]() { MapTest<TOptionalPtr<SimpleObject>, UMockUObject>(&MockObject::m_field); });
                It("should map a const field and return set optional",
                   [this]() { MapTest<TOptionalPtr<const SimpleObject>, UMockUObject>(&MockObject::m_const_field); });
                It("should map a field with const pointer and return set optional",
                   [this]() { MapTest<TOptionalPtr<SimpleObject>, UMockUObject>(&MockObject::m_field_const); });

                It("should map a method with copy parameter and return set optional", [this]() {
                    MapTest<TOptionalPtr<SimpleObject>, UMockUObject>(&MockObject::MethodWithParamCopy, SimpleObject{});
                });
                It("should map a method with pointer parameter and return set optional", [this]() {
                    MapTest<TOptionalPtr<SimpleObject>, UMockUObject>(&MockObject::MethodWithParamPointer,
                                                                      new SimpleObject());
                });
                It("should map a method with const pointer parameter and return set optional", [this]() {
                    MapTest<TOptionalPtr<SimpleObject>, UMockUObject>(&MockObject::MethodWithConstParamPointer,
                                                                      new SimpleObject());
                });
                It("should map a method with const reference parameter and return set optional", [this]() {
                    MapTest<TOptionalPtr<SimpleObject>, UMockUObject>(&MockObject::MethodWithConstParamRef,
                                                                      SimpleObject{});
                });
                It("should map a method with non-const reference parameter and return set optional", [this]() {
                    SimpleObject simple_object{};
                    MapTest<TOptionalPtr<SimpleObject>, UMockUObject>(&MockObject::MethodWithParamRef, simple_object);
                });
                It("should map a method with r-value parameter and return set optional", [this]() {
                    MapTest<TOptionalPtr<SimpleObject>, UMockUObject>(&MockObject::MethodWithParamRValue,
                                                                      SimpleObject{});
                });
                It("should map a method with multiple parameters and return set optional", [this]() {
                    MapTest<TOptionalPtr<SimpleObject>, UMockUObject>(&MockObject::MethodWithMultipleParams,
                                                                      SimpleObject{}, 0, 0.f);
                });

                It("should map the overloaded method 1 and return set optional", [this]() {
                    auto testing_obj =
                        TOptionalPtr<UMockUObject>((UMockUObject *)(m_wrapped_obj))
                            .Map(static_cast<SimpleObject *(MockObject::*)()>(&MockObject::OverloadedMethod));
                    TestTrue("", testing_obj.IsSet());
                    TestTrue("", std::is_same<decltype(testing_obj), TOptionalPtr<SimpleObject>>::value);
                });
                It("should map the overloaded method 2 and return set optional", [this]() {
                    bool executed = false;
                    auto testing_obj =
                        TOptionalPtr<UMockUObject>((UMockUObject *)m_wrapped_obj)
                            .Map(static_cast<SimpleObject *(MockObject::*)(bool &)>(&MockObject::OverloadedMethod),
                                 executed);
                    TestTrue("", testing_obj.IsSet());
                    TestTrue("", executed);
                    TestTrue("", std::is_same<decltype(testing_obj), TOptionalPtr<SimpleObject>>::value);
                });
            });

            Describe("when not valid", [this]() {
                BeforeEach([this]() { m_wrapped_obj = nullptr; });
                It("should map a method and return empty optional",
                   [this]() { MapTest<TOptionalPtr<SimpleObject>, UMockUObject>(&MockObject::Method); });
            });
        });

        Describe("when given a wrapped non-UObject pointer", [this]() {
            Describe("when valid", [this]() {
                BeforeEach([this]() { m_wrapped_obj = new MockNonUObject(); });
                AfterEach([this]() { m_wrapped_obj->Destroy(); });
                It("should map a non-const method and return set optional",
                   [this]() { MapTest<TOptionalPtr<SimpleObject>, MockNonUObject>(&MockObject::Method); });
                It("should map a const method and return set optional",
                   [this]() { MapTest<TOptionalPtr<SimpleObject>, MockNonUObject>(&MockObject::MethodConst); });
                It("should map a method returning const data and return set optional",
                   [this]() { MapTest<TOptionalPtr<const SimpleObject>, MockNonUObject>(&MockObject::ConstMethod); });
                It("should map a const method returning const data and return set optional", [this]() {
                    MapTest<TOptionalPtr<const SimpleObject>, MockNonUObject>(&MockObject::ConstMethodConst);
                });
                It("should map a method returning const pointer and return set optional",
                   [this]() { MapTest<TOptionalPtr<SimpleObject>, MockNonUObject>(&MockObject::ConstPointerMethod); });
                It("should map a const method returning const pointer and return set optional", [this]() {
                    MapTest<TOptionalPtr<SimpleObject>, MockNonUObject>(&MockObject::ConstPointerMethodConst);
                });
                It("should map a non-const field and return set optional",
                   [this]() { MapTest<TOptionalPtr<SimpleObject>, MockNonUObject>(&MockObject::m_field); });
                It("should map a const field and return set optional",
                   [this]() { MapTest<TOptionalPtr<const SimpleObject>, MockNonUObject>(&MockObject::m_const_field); });
                It("should map a field with const pointer and return set optional",
                   [this]() { MapTest<TOptionalPtr<SimpleObject>, MockNonUObject>(&MockObject::m_field_const); });

                It("should map a method with copy parameter and return set optional", [this]() {
                    MapTest<TOptionalPtr<SimpleObject>, MockNonUObject>(&MockObject::MethodWithParamCopy,
                                                                        SimpleObject{});
                });
                It("should map a method with pointer parameter and return set optional", [this]() {
                    MapTest<TOptionalPtr<SimpleObject>, MockNonUObject>(&MockObject::MethodWithParamPointer,
                                                                        new SimpleObject{});
                });
                It("should map a method with const pointer parameter and return set optional", [this]() {
                    MapTest<TOptionalPtr<SimpleObject>, MockNonUObject>(&MockObject::MethodWithConstParamPointer,
                                                                        new SimpleObject{});
                });
                It("should map a method with const reference parameter and return set optional", [this]() {
                    MapTest<TOptionalPtr<SimpleObject>, MockNonUObject>(&MockObject::MethodWithConstParamRef,
                                                                        SimpleObject{});
                });
                It("should map a method with non-const reference parameter and return set optional", [this]() {
                    SimpleObject simple_object{};
                    MapTest<TOptionalPtr<SimpleObject>, MockNonUObject>(&MockObject::MethodWithParamRef, simple_object);
                });
                It("should map a method with r-value parameter and return set optional", [this]() {
                    MapTest<TOptionalPtr<SimpleObject>, MockNonUObject>(&MockObject::MethodWithParamRValue,
                                                                        SimpleObject{});
                });
                It("should map a method with multiple parameters and return set optional", [this]() {
                    MapTest<TOptionalPtr<SimpleObject>, MockNonUObject>(&MockObject::MethodWithMultipleParams,
                                                                        SimpleObject{}, 0, 0.f);
                });

                It("should map the overloaded method 1 and return set optional", [this]() {
                    auto testing_obj =
                        TOptionalPtr<MockNonUObject>((MockNonUObject *)m_wrapped_obj)
                            .Map(static_cast<SimpleObject *(MockObject::*)()>(&MockObject::OverloadedMethod));
                    TestTrue("", testing_obj.IsSet());
                    TestTrue("", std::is_same<decltype(testing_obj), TOptionalPtr<SimpleObject>>::value);
                });
                It("should map the overloaded method 2 and return set optional", [this]() {
                    bool executed = false;
                    auto testing_obj =
                        TOptionalPtr<MockNonUObject>((MockNonUObject *)m_wrapped_obj)
                            .Map(static_cast<SimpleObject *(MockObject::*)(bool &)>(&MockObject::OverloadedMethod),
                                 executed);
                    TestTrue("", testing_obj.IsSet());
                    TestTrue("", executed);
                    TestTrue("", std::is_same<decltype(testing_obj), TOptionalPtr<SimpleObject>>::value);
                });
            });

            Describe("when not valid", [this]() {
                BeforeEach([this]() { m_wrapped_obj = nullptr; });
                It("should map a method and return empty optional",
                   [this]() { MapTest<TOptionalPtr<SimpleObject>, MockNonUObject>(&MockObject::Method); });
            });
        });
    });
    Describe("IfPresent", [this]() {
        Describe("when given a wrapped UObject pointer", [this]() {
            Describe("when valid", [this]() {
                BeforeEach([this]() { m_wrapped_obj = NewObject<UMockUObject>(); });
                AfterEach([this]() { m_wrapped_obj->Destroy(); });
                It("should execute a non-const method",
                   [this]() { IfPresentTest<UMockUObject>(&MockObject::Method2WithParamRef); });

                It("should execute a const method",
                   [this]() { IfPresentTest<UMockUObject>(&MockObject::MethodConstWithParamRef); });
            });

            Describe("when not valid", [this]() {
                BeforeEach([this]() { m_wrapped_obj = nullptr; });

                It("should not execute a non-const method",
                   [this]() { IfPresentTest<UMockUObject>(&MockObject::Method2WithParamRef); });

                It("should not execute a const method",
                   [this]() { IfPresentTest<UMockUObject>(&MockObject::MethodConstWithParamRef); });
            });
        });

        Describe("when given a wrapped non-UObject pointer", [this]() {
            Describe("when valid", [this]() {
                BeforeEach([this]() { m_wrapped_obj = new MockNonUObject(); });
                AfterEach([this]() { m_wrapped_obj->Destroy(); });

                It("should execute a non-const method",
                   [this]() { IfPresentTest<MockNonUObject>(&MockObject::Method2WithParamRef); });

                It("should execute a const method",
                   [this]() { IfPresentTest<MockNonUObject>(&MockObject::MethodConstWithParamRef); });
            });

            Describe("when not valid", [this]() {
                BeforeEach([this]() { m_wrapped_obj = nullptr; });

                It("should not execute a non-const method",
                   [this]() { IfPresentTest<MockNonUObject>(&MockObject::Method2WithParamRef); });

                It("should not execute a const method",
                   [this]() { IfPresentTest<MockNonUObject>(&MockObject::MethodConstWithParamRef); });
            });
        });
    });
    Describe("OrElse", [this]() {
        Describe("when given a wrapped UObject pointer", [this]() {
            Describe("when valid", [this]() {
                BeforeEach([this]() {
                    m_wrapped_obj = NewObject<UMockUObject>();
                    m_default_obj = NewObject<UMockUObject>();
                });
                AfterEach([this]() {
                    m_wrapped_obj->Destroy();
                    m_default_obj->Destroy();
                });

                It("should return the initial wrapped object", [this]() { OrElseTest<UMockUObject, UMockUObject>(); });
            });

            Describe("when not valid", [this]() {
                BeforeEach([this]() {
                    m_wrapped_obj = nullptr;
                    m_default_obj = NewObject<UMockUObject>();
                });
                AfterEach([this]() { m_default_obj->Destroy(); });

                It("should return the wrapped default object", [this]() { OrElseTest<UMockUObject, UMockUObject>(); });
            });
        });
        Describe("when given a wrapped non-UObject pointer", [this]() {
            Describe("when valid", [this]() {
                BeforeEach([this]() {
                    m_wrapped_obj = new MockNonUObject();
                    m_default_obj = new MockNonUObject();
                });
                AfterEach([this]() {
                    m_wrapped_obj->Destroy();
                    m_default_obj->Destroy();
                });

                It("should return the initial wrapped object",
                   [this]() { OrElseTest<MockNonUObject, MockNonUObject>(); });
            });

            Describe("when not valid", [this]() {
                BeforeEach([this]() {
                    m_wrapped_obj = nullptr;
                    m_default_obj = new MockNonUObject();
                });
                AfterEach([this]() { m_default_obj->Destroy(); });

                It("should return the wrapped default object",
                   [this]() { OrElseTest<MockNonUObject, MockNonUObject>(); });
            });
        });
    });
    Describe("MapToValue", [this]() {
        Describe("when given a wrapped UObject pointer", [this]() {
            Describe("when valid", [this]() {
                BeforeEach([this]() { m_wrapped_obj = NewObject<UMockUObject>(); });
                AfterEach([this]() { m_wrapped_obj->Destroy(); });

                It("should map a non-const method and return the value",
                   [this]() { MapToValueTest<SimpleObject *, UMockUObject>(&MockObject::Method); });
                It("should map a const method and return the value",
                   [this]() { MapToValueTest<SimpleObject *, UMockUObject>(&MockObject::MethodConst); });
                It("should map a method returning const data and return the value",
                   [this]() { MapToValueTest<const SimpleObject *, UMockUObject>(&MockObject::ConstMethod); });
                It("should map a const method returning const data and return the value",
                   [this]() { MapToValueTest<const SimpleObject *, UMockUObject>(&MockObject::ConstMethodConst); });
                It("should map a method returning const pointer and return the value",
                   [this]() { MapToValueTest<SimpleObject *, UMockUObject>(&MockObject::ConstPointerMethod); });
                It("should map a const method returning const pointer and return the value",
                   [this]() { MapToValueTest<SimpleObject *, UMockUObject>(&MockObject::ConstPointerMethodConst); });
                It("should map a non-const field and return the value",
                   [this]() { MapToValueTest<SimpleObject *, UMockUObject>(&MockObject::m_field); });
                It("should map a const field and return the value",
                   [this]() { MapToValueTest<const SimpleObject *, UMockUObject>(&MockObject::m_const_field); });
                It("should map a field with const pointer and return the value",
                   [this]() { MapToValueTest<SimpleObject *, UMockUObject>(&MockObject::m_field_const); });

                It("should map the overloaded method 1 and return the value", [this]() {
                    auto wrapped_obj = NewObject<UMockUObject>();
                    auto default_obj = new SimpleObject{MethodEnum::Default};
                    auto testing_obj =
                        TOptionalPtr<UMockUObject>(wrapped_obj)
                            .MapToValue(default_obj,
                                        static_cast<SimpleObject *(MockObject::*)()>(&MockObject::OverloadedMethod));
                    TestEqual("", *testing_obj, *wrapped_obj->OverloadedMethod());
                    TestTrue("", std::is_same<decltype(testing_obj), SimpleObject *>::value);

                    wrapped_obj->Destroy();
                    delete default_obj;
                });
                It("should map the overloaded method 2 and return the value", [this]() {
                    auto wrapped_obj = NewObject<UMockUObject>();
                    auto default_obj = new SimpleObject{MethodEnum::Default};
                    bool executed = false;
                    auto testing_obj = TOptionalPtr<UMockUObject>(wrapped_obj)
                                           .MapToValue(default_obj,
                                                       static_cast<SimpleObject *(MockObject::*)(bool &)>(
                                                           &MockObject::OverloadedMethod),
                                                       executed);
                    TestTrue("", executed);
                    TestEqual("", *testing_obj, *wrapped_obj->OverloadedMethod(executed));
                    TestTrue("", std::is_same<decltype(testing_obj), SimpleObject *>::value);

                    wrapped_obj->Destroy();
                    delete default_obj;
                });
            });

            Describe("when not valid", [this]() {
                BeforeEach([this]() { m_wrapped_obj = nullptr; });

                It("should map a method and return the default value",
                   [this]() { MapToValueTest<SimpleObject *, UMockUObject>(&MockObject::Method); });
            });
        });

        Describe("when given a wrapped non-UObject pointer", [this]() {
            Describe("when valid", [this]() {
                BeforeEach([this]() { m_wrapped_obj = new MockNonUObject(); });
                AfterEach([this]() { m_wrapped_obj->Destroy(); });

                It("should map a non-const method and return the value",
                   [this]() { MapToValueTest<SimpleObject *, MockNonUObject>(&MockObject::Method); });
                It("should map a const method and return the value",
                   [this]() { MapToValueTest<SimpleObject *, MockNonUObject>(&MockObject::MethodConst); });
                It("should map a method returning const data and return the value",
                   [this]() { MapToValueTest<const SimpleObject *, MockNonUObject>(&MockObject::ConstMethod); });
                It("should map a const method returning const data and return the value",
                   [this]() { MapToValueTest<const SimpleObject *, MockNonUObject>(&MockObject::ConstMethodConst); });
                It("should map a method returning const pointer and return the value",
                   [this]() { MapToValueTest<SimpleObject *, MockNonUObject>(&MockObject::ConstPointerMethod); });
                It("should map a const method returning const pointer and return the value",
                   [this]() { MapToValueTest<SimpleObject *, MockNonUObject>(&MockObject::ConstPointerMethodConst); });
                It("should map a non-const field and return the value",
                   [this]() { MapToValueTest<SimpleObject *, MockNonUObject>(&MockObject::m_field); });
                It("should map a const field and return the value",
                   [this]() { MapToValueTest<const SimpleObject *, MockNonUObject>(&MockObject::m_const_field); });
                It("should map a field with const pointer and return the value",
                   [this]() { MapToValueTest<SimpleObject *, MockNonUObject>(&MockObject::m_field_const); });

                It("should map the overloaded method 1 and return the value", [this]() {
                    auto wrapped_obj = new MockNonUObject();
                    auto default_obj = new SimpleObject{MethodEnum::Default};
                    auto testing_obj =
                        TOptionalPtr<MockNonUObject>(wrapped_obj)
                            .MapToValue(default_obj,
                                        static_cast<SimpleObject *(MockObject::*)()>(&MockObject::OverloadedMethod));
                    TestEqual("", *testing_obj, *wrapped_obj->OverloadedMethod());
                    TestTrue("", std::is_same<decltype(testing_obj), SimpleObject *>::value);

                    wrapped_obj->Destroy();
                    delete default_obj;
                });
                It("should map the overloaded method 2 and return the value", [this]() {
                    auto wrapped_obj = new MockNonUObject();
                    auto default_obj = new SimpleObject{MethodEnum::Default};
                    bool executed = false;
                    auto testing_obj = TOptionalPtr<MockNonUObject>(wrapped_obj)
                                           .MapToValue(default_obj,
                                                       static_cast<SimpleObject *(MockObject::*)(bool &)>(
                                                           &MockObject::OverloadedMethod),
                                                       executed);
                    TestTrue("", executed);
                    TestEqual("", *testing_obj, *wrapped_obj->OverloadedMethod(executed));
                    TestTrue("", std::is_same<decltype(testing_obj), SimpleObject *>::value);

                    wrapped_obj->Destroy();
                    delete default_obj;
                });
            });

            Describe("when not valid", [this]() {
                BeforeEach([this]() { m_wrapped_obj = nullptr; });

                It("should map a method and return the default value",
                   [this]() { MapToValueTest<SimpleObject *, MockNonUObject>(&MockObject::Method); });
            });
        });
    });
    Describe("MapStatic", [this]() {
        Describe("when given a wrapped UObject pointer", [this]() {
            Describe("when valid", [this]() {
                BeforeEach([this]() { m_wrapped_obj = NewObject<UMockUObject>(); });
                AfterEach([this]() { m_wrapped_obj->Destroy(); });

                It("should map a static function taking the wrapped object as parameter and return set optional",
                   [this]() { MapStaticTest<TOptionalPtr<SimpleObject>, UMockUObject>(&MockObject::StaticFunction); });
                It("should map a static function taking the wrapped object as parameter and another copy parameter and "
                   "return set optional",
                   [this]() {
                       MapStaticTest<TOptionalPtr<SimpleObject>, UMockUObject>(&MockObject::StaticFunctionWithParamCopy,
                                                                               SimpleObject{});
                   });
                It("should map a static function taking the wrapped object as parameter and another reference "
                   "parameter and return set optional",
                   [this]() {
                       MapStaticTest<TOptionalPtr<SimpleObject>, UMockUObject>(
                           &MockObject::StaticFunctionWithConstParamRef, SimpleObject{});
                   });

                It("should map the overloaded static function 1 and return the value", [this]() {
                    auto wrapped_obj = NewObject<UMockUObject>();
                    auto default_obj = new SimpleObject{MethodEnum::Default};
                    auto testing_obj =
                        TOptionalPtr<UMockUObject>(wrapped_obj)
                            .Map(static_cast<SimpleObject *(*)(MockObject *)>(&MockObject::OverloadedStaticFunction));
                    TestTrue("", testing_obj.IsSet());
                    TestTrue("", std::is_same<decltype(testing_obj), TOptionalPtr<SimpleObject>>::value);

                    wrapped_obj->Destroy();
                    delete default_obj;
                });
                It("should map the overloaded static function 2, set reference bool and return the value", [this]() {
                    auto wrapped_obj = NewObject<UMockUObject>();
                    auto default_obj = new SimpleObject{MethodEnum::Default};
                    bool executed = false;
                    auto testing_obj = TOptionalPtr<UMockUObject>(wrapped_obj)
                                           .Map(static_cast<SimpleObject *(*)(MockObject *, bool &)>(
                                                    &MockObject::OverloadedStaticFunction),
                                                executed);
                    TestTrue("", testing_obj.IsSet());
                    TestTrue("", executed);
                    TestTrue("", std::is_same<decltype(testing_obj), TOptionalPtr<SimpleObject>>::value);

                    wrapped_obj->Destroy();
                    delete default_obj;
                });
            });

            Describe("when not valid", [this]() {
                BeforeEach([this]() { m_wrapped_obj = nullptr; });

                It("should map a function and return empty optional",
                   [this]() { MapStaticTest<TOptionalPtr<SimpleObject>, UMockUObject>(&MockObject::StaticFunction); });
            });
        });

        Describe("when given a wrapped non-UObject pointer", [this]() {
            Describe("when valid", [this]() {
                BeforeEach([this]() { m_wrapped_obj = new MockNonUObject(); });
                AfterEach([this]() { m_wrapped_obj->Destroy(); });

                It("should map a static function taking the wrapped object as parameter and return set optional",
                   [this]() {
                       MapStaticTest<TOptionalPtr<SimpleObject>, MockNonUObject>(&MockObject::StaticFunction);
                   });
                It("should map a static function taking the wrapped object as parameter and another copy parameter and "
                   "return set optional",
                   [this]() {
                       MapStaticTest<TOptionalPtr<SimpleObject>, MockNonUObject>(
                           &MockObject::StaticFunctionWithParamCopy, SimpleObject{});
                   });
                It("should map a static function taking the wrapped object as parameter and another reference "
                   "parameter and return set optional",
                   [this]() {
                       MapStaticTest<TOptionalPtr<SimpleObject>, MockNonUObject>(
                           &MockObject::StaticFunctionWithConstParamRef, SimpleObject{});
                   });

                It("should map the overloaded static function 1 and return the value", [this]() {
                    auto wrapped_obj = new MockNonUObject();
                    auto default_obj = new SimpleObject{MethodEnum::Default};
                    auto testing_obj =
                        TOptionalPtr<MockNonUObject>(wrapped_obj)
                            .Map(static_cast<SimpleObject *(*)(MockObject *)>(&MockObject::OverloadedStaticFunction));
                    TestTrue("", testing_obj.IsSet());
                    TestTrue("", std::is_same<decltype(testing_obj), TOptionalPtr<SimpleObject>>::value);

                    wrapped_obj->Destroy();
                    delete default_obj;
                });
                It("should map the overloaded static function 2, set reference bool and return the value", [this]() {
                    auto wrapped_obj = new MockNonUObject();
                    auto default_obj = new SimpleObject{MethodEnum::Default};
                    auto testing_obj =
                        TOptionalPtr<MockNonUObject>(wrapped_obj)
                            .Map(static_cast<SimpleObject *(*)(MockObject *)>(&MockObject::OverloadedStaticFunction));
                    TestTrue("", testing_obj.IsSet());
                    TestTrue("", std::is_same<decltype(testing_obj), TOptionalPtr<SimpleObject>>::value);

                    wrapped_obj->Destroy();
                    delete default_obj;
                });
            });

            Describe("when not valid", [this]() {
                BeforeEach([this]() { m_wrapped_obj = nullptr; });

                It("should map a static function and return empty optional", [this]() {
                    MapStaticTest<TOptionalPtr<SimpleObject>, MockNonUObject>(&MockObject::StaticFunction);
                });
            });
        });
    });
}

BEGIN_DEFINE_SPEC(FOptionalPtrPerformanceSpec, "OptionalPtr.Performance",
                  EAutomationTestFlags::ProductFilter | EAutomationTestFlags::EditorContext)

// https://stackoverflow.com/a/33900479
typedef std::chrono::high_resolution_clock::time_point TimeVar;

template <std::derived_from<UObject> MockType>
MockType *GetObject()
{
    return NewObject<MockType>();
}

template <typename MockType>
    requires(!std::derived_from<MockType, UObject>)
MockType *GetObject(bool dont_set = false)
{
    return new MockType();
}

template <typename MockType, typename F>
double GetExecutionTime(uint32 reps, F func)
{
    using namespace std::chrono;

    MockType *obj1 = GetObject<MockType>();
    MockType *obj2 = GetObject<MockType>();

    const TimeVar time_now = high_resolution_clock::now();
    for (uint32 i = 0; i < reps; ++i)
    {
        // has to be member function template, specialized static function template won't compile on PS4
        (this->*func)(obj1, obj2);
    }
    const auto exec_time = duration_cast<nanoseconds>(high_resolution_clock::now() - time_now).count();

    obj1->Destroy();
    obj2->Destroy();

    return exec_time;
}
// END

template <uint8 NumOfCalls>
UMockUObject *RegularFlowUObject(UMockUObject *obj1, UMockUObject *obj2);

template <>
UMockUObject *RegularFlowUObject<1>(UMockUObject *obj1, UMockUObject *obj2)
{
    UMockUObject *obj = obj1;
    UMockUObject *result = nullptr;

    if (IsValid(obj))
        result = obj->GetRandomObject(obj1, obj2);

    return result;
}

template <>
UMockUObject *RegularFlowUObject<2>(UMockUObject *obj1, UMockUObject *obj2)
{
    UMockUObject *obj = obj1;
    UMockUObject *result = nullptr;

    if (!IsValid(obj))
        return nullptr;

    auto result1 = obj->GetRandomObject(obj1, obj2);
    if (!IsValid(result1))
        return nullptr;

    result = result1->GetRandomObject(obj1, obj2);

    return result;
}

template <>
UMockUObject *RegularFlowUObject<3>(UMockUObject *obj1, UMockUObject *obj2)
{
    UMockUObject *obj = obj1;
    UMockUObject *result = nullptr;

    if (!IsValid(obj))
        return nullptr;

    auto result1 = obj->GetRandomObject(obj1, obj2);
    if (!IsValid(result1))
        return nullptr;

    auto result2 = result1->GetRandomObject(obj1, obj2);
    if (!IsValid(result2))
        return nullptr;

    result = result2->GetRandomObject(obj1, obj2);

    return result;
}

template <>
UMockUObject *RegularFlowUObject<4>(UMockUObject *obj1, UMockUObject *obj2)
{
    UMockUObject *obj = obj1;
    UMockUObject *result = nullptr;

    if (!IsValid(obj))
        return nullptr;

    auto result1 = obj->GetRandomObject(obj1, obj2);
    if (!IsValid(result1))
        return nullptr;

    auto result2 = result1->GetRandomObject(obj1, obj2);
    if (!IsValid(result2))
        return nullptr;

    auto result3 = result2->GetRandomObject(obj1, obj2);
    if (!IsValid(result3))
        return nullptr;

    result = result3->GetRandomObject(obj1, obj2);

    return result;
}

template <uint8 NumOfCalls>
MockNonUObject *RegularFlowNonUObject(MockNonUObject *obj1, MockNonUObject *obj2);

template <>
MockNonUObject *RegularFlowNonUObject<1>(MockNonUObject *obj1, MockNonUObject *obj2)
{
    MockNonUObject *obj = obj1;
    MockNonUObject *result = nullptr;

    if (obj != nullptr)
        result = obj->GetRandomObject(obj1, obj2);

    return result;
}

template <>
MockNonUObject *RegularFlowNonUObject<2>(MockNonUObject *obj1, MockNonUObject *obj2)
{
    MockNonUObject *obj = obj1;

    if (obj == nullptr)
        return nullptr;

    auto result1 = obj->GetRandomObject(obj1, obj2);
    if (result1 == nullptr)
        return nullptr;

    return result1->GetRandomObject(obj1, obj2);
}

template <>
MockNonUObject *RegularFlowNonUObject<3>(MockNonUObject *obj1, MockNonUObject *obj2)
{
    MockNonUObject *obj = obj1;

    if (obj == nullptr)
        return nullptr;

    auto result1 = obj->GetRandomObject(obj1, obj2);
    if (result1 == nullptr)
        return nullptr;

    auto result2 = obj->GetRandomObject(obj1, obj2);
    if (result2 == nullptr)
        return nullptr;

    return result2->GetRandomObject(obj1, obj2);
}

template <>
MockNonUObject *RegularFlowNonUObject<4>(MockNonUObject *obj1, MockNonUObject *obj2)
{
    MockNonUObject *obj = obj1;

    if (obj == nullptr)
        return nullptr;

    auto result1 = obj->GetRandomObject(obj1, obj2);
    if (result1 == nullptr)
        return nullptr;

    auto result2 = obj->GetRandomObject(obj1, obj2);
    if (result2 == nullptr)
        return nullptr;

    auto result3 = obj->GetRandomObject(obj1, obj2);
    if (result3 == nullptr)
        return nullptr;

    return result3->GetRandomObject(obj1, obj2);
}

template <uint8 NumOfCalls>
UMockUObject *MapFlowUObject(UMockUObject *obj1, UMockUObject *obj2);

template <>
UMockUObject *MapFlowUObject<1>(UMockUObject *obj1, UMockUObject *obj2)
{
    return TOptionalPtr<UMockUObject>(obj1).Map(&UMockUObject::GetRandomObject, obj1, obj2).Get();
}

template <>
UMockUObject *MapFlowUObject<2>(UMockUObject *obj1, UMockUObject *obj2)
{
    return TOptionalPtr<UMockUObject>(obj1)
        .Map(&UMockUObject::GetRandomObject, obj1, obj2)
        .Map(&UMockUObject::GetRandomObject, obj1, obj2)
        .Get();
}

template <>
UMockUObject *MapFlowUObject<3>(UMockUObject *obj1, UMockUObject *obj2)
{
    return TOptionalPtr<UMockUObject>(obj1)
        .Map(&UMockUObject::GetRandomObject, obj1, obj2)
        .Map(&UMockUObject::GetRandomObject, obj1, obj2)
        .Map(&UMockUObject::GetRandomObject, obj1, obj2)
        .Get();
}

template <>
UMockUObject *MapFlowUObject<4>(UMockUObject *obj1, UMockUObject *obj2)
{
    return TOptionalPtr<UMockUObject>(obj1)
        .Map(&UMockUObject::GetRandomObject, obj1, obj2)
        .Map(&UMockUObject::GetRandomObject, obj1, obj2)
        .Map(&UMockUObject::GetRandomObject, obj1, obj2)
        .Map(&UMockUObject::GetRandomObject, obj1, obj2)
        .Get();
}

template <uint8 NumOfCalls>
MockNonUObject *MapFlowNonUObject(MockNonUObject *obj1, MockNonUObject *obj2);

template <>
MockNonUObject *MapFlowNonUObject<1>(MockNonUObject *obj1, MockNonUObject *obj2)
{
    return TOptionalPtr<MockNonUObject>(obj1).Map(&MockNonUObject::GetRandomObject, obj1, obj2).Get();
}

template <>
MockNonUObject *MapFlowNonUObject<2>(MockNonUObject *obj1, MockNonUObject *obj2)
{
    return TOptionalPtr<MockNonUObject>(obj1)
        .Map(&MockNonUObject::GetRandomObject, obj1, obj2)
        .Map(&MockNonUObject::GetRandomObject, obj1, obj2)
        .Get();
}

template <>
MockNonUObject *MapFlowNonUObject<3>(MockNonUObject *obj1, MockNonUObject *obj2)
{
    return TOptionalPtr<MockNonUObject>(obj1)
        .Map(&MockNonUObject::GetRandomObject, obj1, obj2)
        .Map(&MockNonUObject::GetRandomObject, obj1, obj2)
        .Map(&MockNonUObject::GetRandomObject, obj1, obj2)
        .Get();
}

template <>
MockNonUObject *MapFlowNonUObject<4>(MockNonUObject *obj1, MockNonUObject *obj2)
{
    return TOptionalPtr<MockNonUObject>(obj1)
        .Map(&MockNonUObject::GetRandomObject, obj1, obj2)
        .Map(&MockNonUObject::GetRandomObject, obj1, obj2)
        .Map(&MockNonUObject::GetRandomObject, obj1, obj2)
        .Map(&MockNonUObject::GetRandomObject, obj1, obj2)
        .Get();
}

const static uint32 num_of_repetitions = 100000;

template <uint8 NumOfCalls, bool IsUObject, typename MockType>
    requires IsUObject
double GetExecTimeInternal()
{
    return GetExecutionTime<MockType>(num_of_repetitions, &FOptionalPtrPerformanceSpec::RegularFlowUObject<NumOfCalls>);
}

template <uint8 NumOfCalls, bool IsUObject, typename MockType>
    requires !IsUObject
             double GetExecTimeInternal(bool dont_set = false)
{
    return GetExecutionTime<MockType>(num_of_repetitions,
                                      &FOptionalPtrPerformanceSpec::RegularFlowNonUObject<NumOfCalls>);
}

template <uint8 NumOfCalls, bool IsUObject = true>
    requires IsUObject
void CompareExecutionTimes()
{
    const auto map_exec_time =
        GetExecutionTime<UMockUObject>(num_of_repetitions, &FOptionalPtrPerformanceSpec::MapFlowUObject<NumOfCalls>);
    AddInfo(FString::Printf(TEXT("Execution time for Map approach: %f ns"), map_exec_time));
    AddInfo(FString::Printf(TEXT("Execution time for regular approach: %f ns"),
                            GetExecTimeInternal<NumOfCalls, IsUObject, UMockUObject>()));
}

template <uint8 NumOfCalls, bool IsUObject = true>
    requires !IsUObject
             void CompareExecutionTimes(bool dont_set = false)
{
    const auto map_exec_time = GetExecutionTime<MockNonUObject>(
        num_of_repetitions, &FOptionalPtrPerformanceSpec::MapFlowNonUObject<NumOfCalls>);
    AddInfo(FString::Printf(TEXT("Execution time for Map approach: %f ns"), map_exec_time));
    AddInfo(FString::Printf(TEXT("Execution time for regular approach: %f ns"),
                            GetExecTimeInternal<NumOfCalls, IsUObject, MockNonUObject>()));
}
END_DEFINE_SPEC(FOptionalPtrPerformanceSpec)

void FOptionalPtrPerformanceSpec::Define()
{
    Describe("Map", [this]() {
        Describe("when given a wrapped UObject pointer", [this]() {
            It(FString::Printf(TEXT("should log the performance for 1 call with validation over %u repetitions"),
                               num_of_repetitions),
               [this]() { CompareExecutionTimes<1>(); });
            It(FString::Printf(TEXT("should log the performance for 2 calls with validation over %u repetitions"),
                               num_of_repetitions),
               [this]() { CompareExecutionTimes<2>(); });
            It(FString::Printf(TEXT("should log the performance for 3 calls with validation over %u repetitions"),
                               num_of_repetitions),
               [this]() { CompareExecutionTimes<3>(); });
            It(FString::Printf(TEXT("should log the performance for 4 calls with validation over %u repetitions"),
                               num_of_repetitions),
               [this]() { CompareExecutionTimes<4>(); });
        });
        Describe("when given a wrapped non-UObject pointer", [this]() {
            It(FString::Printf(TEXT("should log the performance for 1 call with validation over %u repetitions"),
                               num_of_repetitions),
               [this]() { CompareExecutionTimes<1, false>(); });
            It(FString::Printf(TEXT("should log the performance for 2 calls with validation over %u repetitions"),
                               num_of_repetitions),
               [this]() { CompareExecutionTimes<2, false>(); });
            It(FString::Printf(TEXT("should log the performance for 3 calls with validation over %u repetitions"),
                               num_of_repetitions),
               [this]() { CompareExecutionTimes<3, false>(); });
            It(FString::Printf(TEXT("should log the performance for 4 calls with validation over %u repetitions"),
                               num_of_repetitions),
               [this]() { CompareExecutionTimes<4, false>(); });
        });
    });
}