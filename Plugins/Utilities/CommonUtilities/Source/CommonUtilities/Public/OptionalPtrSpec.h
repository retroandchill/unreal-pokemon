#pragma once

#include "CoreMinimal.h"

#include "OptionalPtrSpec.generated.h"

class MockUtils;

enum class MethodEnum
{
    Method,
    MethodConst,
    ConstMethod,
    ConstMethodConst,
    ConstPointerMethod,
    ConstPointerMethodConst,
    OverloadedMethod,
    Default
};

class SimpleObject
{
  public:
    const MethodEnum func_name;

    SimpleObject() : func_name(MethodEnum::Default)
    {
    }

    SimpleObject(const MethodEnum &func_name) : func_name{func_name}
    {
    }

    bool operator==(const SimpleObject &rhs) const
    {
        return func_name == rhs.func_name;
    }
    bool operator!=(const SimpleObject &rhs) const
    {
        return !operator==(rhs);
    }
};

class COMMONUTILITIES_API MockObject
{
    mutable TArray<SimpleObject *> allocated;

    SimpleObject *Create(MethodEnum method_enum = MethodEnum::Default) const
    {
        auto simple_object = new SimpleObject(method_enum);
        allocated.Add(simple_object);
        return simple_object;
    }

  public:
    virtual ~MockObject()
    {
        for (const auto elem : allocated)
        {
            delete elem;
        }
    }

    virtual void Destroy() = 0;

    SimpleObject *Method()
    {
        return Create(MethodEnum::Method);
    }
    SimpleObject *MethodConst() const
    {
        return Create(MethodEnum::MethodConst);
    }
    const SimpleObject *ConstMethod()
    {
        return Create(MethodEnum::ConstMethod);
    }
    const SimpleObject *ConstMethodConst() const
    {
        return Create(MethodEnum::ConstMethodConst);
    }
    SimpleObject *const ConstPointerMethod()
    {
        return Create(MethodEnum::ConstPointerMethod);
    }
    SimpleObject *const ConstPointerMethodConst() const
    {
        return Create(MethodEnum::ConstPointerMethodConst);
    }

    SimpleObject *MethodWithParamCopy(SimpleObject)
    {
        return Create();
    }
    SimpleObject *MethodWithParamPointer(SimpleObject *)
    {
        return Create();
    }
    SimpleObject *MethodWithConstParamPointer(const SimpleObject *)
    {
        return Create();
    }
    SimpleObject *MethodWithConstParamRef(const SimpleObject &)
    {
        return Create();
    }
    SimpleObject *MethodWithParamRef(SimpleObject &)
    {
        return Create();
    }
    SimpleObject *MethodWithParamRValue(SimpleObject &&)
    {
        return Create();
    }
    SimpleObject *MethodWithMultipleParams(SimpleObject, int, float)
    {
        return Create();
    }

    SimpleObject *OverloadedMethod()
    {
        return Create(MethodEnum::OverloadedMethod);
    }
    SimpleObject *OverloadedMethod(bool &executed)
    {
        executed = true;
        return Create(MethodEnum::OverloadedMethod);
    }

    void MethodConstWithParamRef(bool &method_executed) const
    {
        method_executed = true;
    }
    void Method2WithParamRef(bool &method_executed)
    {
        method_executed = true;
    }

    static SimpleObject *StaticFunction(MockObject *)
    {
        return new SimpleObject();
    }
    static SimpleObject *StaticFunctionWithParamCopy(MockObject *, SimpleObject)
    {
        return new SimpleObject();
    }
    static SimpleObject *StaticFunctionWithConstParamRef(MockObject *, const SimpleObject &)
    {
        return new SimpleObject();
    }

    static SimpleObject *OverloadedStaticFunction(MockObject *)
    {
        return new SimpleObject();
    }
    static SimpleObject *OverloadedStaticFunction(MockObject *, bool &executed)
    {
        executed = true;
        return new SimpleObject();
    }

    SimpleObject *m_field = Create();
    const SimpleObject *m_const_field = Create();
    SimpleObject *const m_field_const = Create();
};

UCLASS()
class COMMONUTILITIES_API UMockUObject : public UObject, public MockObject
{
    class MockUtils;

    GENERATED_BODY()
  public:
    UMockUObject *GetRandomObject(UMockUObject *obj1, UMockUObject *obj2)
    {
        return FMath::RandBool() ? obj1 : obj2;
    }

    virtual void Destroy() override
    {
        this->ConditionalBeginDestroy();
    }
};

class COMMONUTILITIES_API MockNonUObject : public MockObject
{
  public:
    MockNonUObject *GetRandomObject(MockNonUObject *obj1, MockNonUObject *obj2)
    {
        return FMath::RandBool() ? obj1 : obj2;
    }

    virtual void Destroy() override
    {
        delete this;
    }
};