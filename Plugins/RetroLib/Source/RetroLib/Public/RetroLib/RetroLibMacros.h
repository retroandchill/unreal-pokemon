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

#define RETROLIB_FUNCTIONAL_EXTENSION(Exporter, Method, Name) \
  constexpr auto Invoker_##Name##_Method_Variable = Method; \
  template <auto Functor = DynamicFunctor> \
        requires (DynamicFunctorBinding<Functor> || IsValidFunctorObject(Functor)) \
  constexpr TFunctorBindingInvoker<Functor, Invoker_##Name##_Method_Variable> FunctorExtension_##Name##_Callback; \
  Exporter template <auto Functor = DynamicFunctor, typename... A> \
        requires (DynamicFunctorBinding<Functor> || IsValidFunctorObject(Functor)) \
    constexpr auto Name(A &&...Args) { \
          return ExtensionMethod<FunctorExtension_##Name##_Callback<Functor>>(std::forward<A>(Args)...); \
    }

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
#define RETRO_DECLARE_VARIANT_OBJECT_STRUCT(StructName, ...)                                                              \
    struct FSoft##StructName;                                                                                          \
    struct F##StructName : Retro::TVariantObject<__VA_ARGS__> {                                                   \
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
    struct Retro::Detail::TIsVariantObject<F##StructName> : std::true_type {};                                    \
    struct FSoft##StructName : Retro::TSoftVariantObject<F##StructName> {                                         \
        FSoft##StructName() = default;                                                                                 \
        template <typename... T>                                                                                       \
            requires std::constructible_from<TSoftVariantObject, T...>                                                 \
        explicit FSoft##StructName(T &&...Args) : TSoftVariantObject(std::forward<T>(Args)...) {                       \
        }                                                                                                              \
    };                                                                                                                 \
    template <>                                                                                                        \
    struct Retro::Detail::TIsSoftVariantObject<FSoft##StructName> : std::true_type {}

/**
 * Perform the static registration of the struct type. This is required to allow a variant struct to be accessible to
 * blueprints.
 * @param StructName The name of the struct to implement.
 */
#define RETRO_DEFINE_VARIANT_OBJECT_STRUCT(StructName)                                                                    \
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
        check(false) Retro::Unreachable();                                                                        \
    }



#define NUMBER_LITERAL(Number) #Number

#define TEXT_LITERAL(Text) NSLOCTEXT(__FILE__, __FUNCTION__ NUMBER_LITERAL(__LINE__), Text)

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define ABSTRACT_METHOD {                                                                                                                  \
        LowLevelFatalError(TEXT("Pure virtual not implemented (%s)"), TEXT(__PRETTY_FUNCTION__));                      \
        Retro::Unreachable();                                                                                     \
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
#endif