/**
 * @file RttiCheck.h
 * @brief Contains the basic info for checking if RTTI is enabled.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include <cassert>
#endif

#if defined(__clang__)
#if __has_feature(cxx_rtti)
#define RTTI_ENABLED 1
#endif
#elif defined(__GNUG__)
#if defined(__GXX_RTTI)
#define RTTI_ENABLED 1
#endif
#elif defined(_MSC_VER)
#if defined(_CPPRTTI)
#define RTTI_ENABLED 1
#endif
#endif

#ifndef RTTI_ENABLED
#define RTTI_ENABLED 0
#endif

#ifdef __UNREAL__
#define RETROLIB_ASSERT(...) check(__VA_ARGS__)
#else
#define RETROLIB_ASSERT(...) assert(__VA_ARGS__)
#endif

#ifdef __UNREAL__
#define RETROLIB_DEFAULT_OPTIONAL_TYPE TOptional
#else
#define RETROLIB_DEFAULT_OPTIONAL_TYPE std::optional
#endif

#ifdef __UNREAL__
/**
 * Declare a new variant object struct with the given name
 * @param StructName The name of the struct in question
 * @param ... The types that are registered to the struct type
 */
#define RETRO_DECLARE_VARIANT_OBJECT_STRUCT(StructName, ...)                                                           \
    struct FSoft##StructName;                                                                                          \
    struct F##StructName : Retro::TVariantObject<__VA_ARGS__> {                                                        \
        using SoftPtrType = FSoft##StructName;                                                                         \
        F##StructName() = default;                                                                                     \
        template <typename... T>                                                                                       \
            requires std::constructible_from<TVariantObject, T...>                                                     \
        explicit F##StructName(T &&...Args) : TVariantObject(std::forward<T>(Args)...) {                               \
        }                                                                                                              \
        void Reset() {                                                                                                 \
            SetUnchecked(nullptr);                                                                                     \
        }                                                                                                              \
    };                                                                                                                 \
    template <>                                                                                                        \
    struct Retro::TIsVariantObject<F##StructName> : std::true_type {};                                                 \
    struct FSoft##StructName : Retro::TSoftVariantObject<F##StructName> {                                              \
        FSoft##StructName() = default;                                                                                 \
        template <typename... T>                                                                                       \
            requires std::constructible_from<TSoftVariantObject, T...>                                                 \
        explicit FSoft##StructName(T &&...Args) : TSoftVariantObject(std::forward<T>(Args)...) {                       \
        }                                                                                                              \
    };                                                                                                                 \
    template <>                                                                                                        \
    struct Retro::TIsSoftVariantObject<FSoft##StructName> : std::true_type {}

/**
 * Perform the static registration of the struct type. This is required to allow a variant struct to be accessible to
 * blueprints.
 * @param StructName The name of the struct to implement.
 */
#define RETRO_DEFINE_VARIANT_OBJECT_STRUCT(StructName)                                                                 \
    static const bool __##StructName__Registration =                                                                   \
        Retro::FVariantObjectStructRegistry::RegisterVariantStruct<StructName>()

#define P_GET_WILDCARD_PARAM(PropVar, PointerVar)                                                                      \
    Stack.StepCompiledIn<FProperty>(nullptr);                                                                          \
    const auto PropVar = Stack.MostRecentProperty;                                                                     \
    auto PointerVar = Stack.MostRecentPropertyAddress;

#define P_GET_OPAQUE_STRUCT(PropVar, PointerVar)                                                                       \
    Stack.StepCompiledIn<FStructProperty>(nullptr);                                                                    \
    const auto PropVar = CastField<FStructProperty>(Stack.MostRecentProperty);                                         \
    auto PointerVar = Stack.MostRecentPropertyAddress;

#define P_GET_RESULT(Type, Name) auto &Name = *static_cast<Type *>(RESULT_PARAM)

#define CUSTOM_THUNK_STUB(RetType, Method, ...)                                                                        \
    RetType Method(__VA_ARGS__) {                                                                                      \
        check(false) Retro::Unreachable();                                                                             \
    }

#define NUMBER_LITERAL(Number) #Number

#define TEXT_LITERAL(Text) NSLOCTEXT(__FILE__, __FUNCTION__ NUMBER_LITERAL(__LINE__), Text)

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define ABSTRACT_METHOD                                                                                                \
    {                                                                                                                  \
        LowLevelFatalError(TEXT("Pure virtual not implemented (%s)"), TEXT(__PRETTY_FUNCTION__));                      \
        Retro::Unreachable();                                                                                          \
    }

#define NUMBER_LITERAL(Number) #Number

#define TEXT_LITERAL(Text) NSLOCTEXT(__FILE__, __FUNCTION__ NUMBER_LITERAL(__LINE__), Text)

/**
 * Macro for handling creating localized text, using the filename for the namespace.
 *
 * @param Key The searchable text key
 * @param Text The text itself
 */
#define LOCALIZED_TEXT(Key, Text) NSLOCTEXT(__FILE__, Key, Text)

/**
 * Declares a multicast delegate member for a class adding some basic boilerplate methods that are used to bind the
 * delegate and release the binding, but allowing the invocation to remain private.
 * @param DelegateType The type of delegate
 * @param MemberName The name of the member variable
 */
#define RETRO_MULTICAST_DELEGATE_MEMBER(DelegateType, MemberName)                                                      \
  private:                                                                                                             \
    DelegateType MemberName;                                                                                           \
  protected: \
    DelegateType& Get##MemberName() { \
        return MemberName; \
    } \
    const DelegateType& Get##MemberName() const { \
        return MemberName; \
    } \
  public:                                                                                                              \
    template <typename F, typename... A>                                                                               \
        requires Retro::Delegates::CanAddFree<DelegateType, F, A...>                                                   \
    FDelegateHandle BindTo##MemberName(F &&Functor, A &&...Args) {                                                     \
        return MemberName | Retro::Delegates::Add(std::forward<F>(Functor), std::forward<A>(Args)...);                 \
    }                                                                                                                  \
    template <typename O, typename F, typename... A>                                                                   \
        requires Retro::Delegates::CanAddMember<DelegateType, O, F, A...>                                              \
    FDelegateHandle BindTo##MemberName(O &&Object, F &&Functor, A &&...Args) {                                         \
        return MemberName |                                                                                            \
               Retro::Delegates::Add(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);     \
    }                                                                                                                  \
    void RemoveFrom##MemberName(FDelegateHandle Handle) {                                                              \
        MemberName.Remove(Handle);                                                                                     \
    }

#if WITH_EDITOR
/**
 * Helper macro for binding to a delegate or using the constant value when constructing the underlying SWidget.
 * These macros create a binding that has a layer of indirection that allows blueprint debugging to work more
 * effectively.
 */
#define PROPERTY_BINDING_WRAPPED(ReturnType, MemberName)                                                               \
    (MemberName##Delegate.IsBound() && !IsDesignTime()) ? BIND_UOBJECT_ATTRIBUTE(ReturnType, K2_Gate_##MemberName)     \
                                                        : TAttribute<ReturnType>(Get##MemberName())

#define BITFIELD_PROPERTY_BINDING_WRAPPED(MemberName)                                                                  \
    (MemberName##Delegate.IsBound() && !IsDesignTime()) ? BIND_UOBJECT_ATTRIBUTE(bool, K2_Gate_##MemberName)           \
                                                        : TAttribute<bool>(Get##MemberName() != 0)

#else

#define PROPERTY_BINDING_WRAPPED(ReturnType, MemberName)                                                               \
    (MemberName##Delegate.IsBound() && !IsDesignTime())                                                                \
        ? TAttribute<ReturnType>::Create(MemberName##Delegate.GetUObject(), MemberName##Delegate.GetFunctionName())    \
        : TAttribute<ReturnType>(Get##MemberName())

#define BITFIELD_PROPERTY_BINDING_WRAPPED(MemberName)                                                                  \
    (MemberName##Delegate.IsBound() && !IsDesignTime())                                                                \
        ? TAttribute<bool>::Create(MemberName##Delegate.GetUObject(), MemberName##Delegate.GetFunctionName())          \
        : TAttribute<bool>(Get##MemberName() != 0)

#endif

#define DECLARE_STATIC_REGISTRY(Export, Name, ...)                                                                     \
    class Export Name : public __VA_ARGS__ {                                                                           \
        Name() = default;                                                                                              \
        ~Name() = default;                                                                                             \
                                                                                                                       \
      public:                                                                                                          \
        static Name &GetInstance();                                                                                    \
    };

#define DEFINE_STATIC_REGISTRY(Name)                                                                                   \
    Name &Name::GetInstance() {                                                                                        \
        static Name Instance;                                                                                          \
        return Instance;                                                                                               \
    }

#define DECLARE_ABSTRACT_METATYPE                                                                                      \
  public:                                                                                                              \
    virtual FName GetClassName() const = 0;                                                                            \
    static FName ClassName();                                                                                          \
                                                                                                                       \
  private:

#define IMPLEMENT_ABSTRACT_METATYPE(Class)                                                                             \
    FName Class::ClassName() {                                                                                         \
        static const FName ClassName = TEXT(#Class);                                                                   \
        return ClassName;                                                                                              \
    }
#define DECLARE_DERIVED_METATYPE                                                                                       \
  public:                                                                                                              \
    FName GetClassName() const override;                                                                               \
    static FName ClassName();                                                                                          \
                                                                                                                       \
  private:

#define IMPLEMENT_DERIVED_METATYPE(Class)                                                                              \
    FName Class::GetClassName() const {                                                                                \
        return ClassName();                                                                                            \
    }                                                                                                                  \
    FName Class::ClassName() {                                                                                         \
        static const FName ClassName = TEXT(#Class);                                                                   \
        return ClassName;                                                                                              \
    }

#endif