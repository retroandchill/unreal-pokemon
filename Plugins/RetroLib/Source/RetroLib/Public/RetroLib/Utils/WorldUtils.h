// "Unreal Pokémon" created by Retro & Chill.

#pragma once
#include "Kismet/GameplayStatics.h"

#ifdef __UNREAL__
namespace Retro {
    template <typename T>
    concept HasGetWorld = requires(T &&Obj) {
        { std::forward<T>(Obj).GetWorld() } -> std::convertible_to<UWorld*>;
    };

    template <typename T>
    concept HasGetWorldPtr = requires(T &&Obj) {
        { *std::forward<T>(Obj) } -> HasGetWorld;
    };
    
    struct FGetWorldInvoker {

        template <HasGetWorld T>
        constexpr UWorld* operator()(T&& Obj) const {
            return std::forward<T>(Obj).GetWorld();
        }

        template <HasGetWorldPtr T>
        constexpr UWorld* operator()(T&& Obj) const {
            check(IsValidPtr(Obj))
            return (*this)(*std::forward<T>(Obj));
        }
    };

    constexpr FGetWorldInvoker GetWorld;

    template <typename T>
    concept HasGetGameInstance = requires(T &&Obj) {
        { std::forward<T>(Obj).GetGameInstance() } -> std::convertible_to<UGameInstance*>;
    };

    template <typename T>
    concept HasGetGameInstancePtr = requires(T &&Obj) {
        { *std::forward<T>(Obj) } -> HasGetGameInstance;
    };

    struct FGetGameInstanceInvoker {

        template <HasGetGameInstance T>
        constexpr UGameInstance* operator()(T&& Obj) const {
            return std::forward<T>(Obj).GetGameInstance();
        }

        template <HasGetGameInstancePtr T>
        constexpr UGameInstance* operator()(T&& Obj) const {
            check(IsValidPtr(Obj))
            return (*this)(*std::forward<T>(Obj));
        }

        UGameInstance* operator()(const UObject* Obj) const {
            return UGameplayStatics::GetGameInstance(Obj);
        }

        UGameInstance* operator()(const UObject& Obj) const {
            return UGameplayStatics::GetGameInstance(&Obj);
        }
    };

    constexpr FGetGameInstanceInvoker GetGameInstance;

    template <typename T, typename S>
    concept HasGetSubsystem = std::derived_from<S, USubsystem> && requires(T &&Obj) {
        { std::forward<T>(Obj).template GetSubsystem<S>() } -> std::convertible_to<S*>;
    };

    template <typename T, typename S>
    concept HasGetSubsystemPtr = std::derived_from<S, USubsystem> && requires(T &&Obj) {
        { *std::forward<T>(Obj) } -> HasGetSubsystem;
    };

    template <std::derived_from<USubsystem> S>
    struct TGetSubsystemInvoker {

        template <HasGetSubsystem<S> T>
        constexpr auto operator()(T&& Obj) const {
            return std::forward<T>(Obj).template GetSubsystem<S>();
        }

        template <HasGetSubsystemPtr<S> T>
        constexpr auto operator()(T&& Obj) const {
            check(IsValidPtr(Obj))
            return (*this)(*std::forward<T>(Obj));
        }
    };

    template <std::derived_from<USubsystem> S>
    constexpr TGetSubsystemInvoker<S> GetSubsystem;
}
#endif
