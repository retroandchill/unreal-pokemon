// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Functional/FunctionPointers.h"
#include <array>

namespace UE::Ranges {
    class FWildcard final {
    public:

    private:
        union FStorageUnion {
            static constexpr size_t SmallStorageSize = sizeof(void *) * 2;
            using FStackStorage = std::array<std::byte, SmallStorageSize>;

            void *Dynamic;
            FStackStorage Stack;
        };

        struct FVTableType {
            TFunctionPtr<const std::type_info&() noexcept> Function;
            TFunctionPtr<void(FStorageUnion &) noexcept> Destroy;
            TFunctionPtr<void(const FStorageUnion &Src, FStorageUnion &Dest)> Copy;
            TFunctionPtr<void(FStorageUnion &Src, FStorageUnion &Dest) noexcept> Move;
            TFunctionPtr<void(FStorageUnion &Src, FStorageUnion &Dest) noexcept> Swap;
        };

        template <typename T>
        struct TVTableDynamic {
            static const std::type_info &Type() noexcept {
                return typeid(T);
            }

            static void Destroy(FStorageUnion &Storage) noexcept {
                delete static_cast<T *>(Storage.Dynamic);
            }

            static void Copy(const FStorageUnion &Src, FStorageUnion &Dest) {
                Dest.Dynamic = new T(*static_cast<const T *>(Src.Dynamic));
            }

            static void Move(FStorageUnion &Src, FStorageUnion &Dest) noexcept {
                Dest.Dynamic = Src.Dynamic;
                Src.Dynamic = nullptr;
            }

            static void Swap(FStorageUnion &LHS, FStorageUnion &RHS) noexcept {
                std::swap(LHS.Dynamic, RHS.Dynamic);
            }
        };

        template <typename T>
        struct TVTableStack {
            static const std::type_info &Type() noexcept {
                return typeid(T);
            }

            static void Destroy(FStorageUnion &Storage) noexcept {
                reinterpret_cast<T *>(&Storage.Stack)->~T();
            }

            static void Copy(const FStorageUnion &Src, FStorageUnion &Dest) {
                new(&Dest.Stack) T(reinterpret_cast<const T &>(Src.Stack));
            }

            static void Move(FStorageUnion &Src, FStorageUnion &Dest) noexcept {
                new(&Dest.Stack) T(std::move(reinterpret_cast<const T &>(Src.Stack)));
                Destroy(Src);
            }

            static void Swap(FStorageUnion &LHS, FStorageUnion &RHS) noexcept {
                FStorageUnion Temp;
                Move(RHS, Temp);
                Move(LHS, RHS);
                Move(Temp, LHS);
            }
        };

        template <typename T>
        static constexpr bool bRequiresAllocation = !(std::is_nothrow_move_constructible_v<T>
                                                      && sizeof(T) <= sizeof(FStorageUnion::Stack)
                                                      && alignof(T) <= alignof(FStorageUnion::FStackStorage));

        template <typename T>
        static FVTableType* VTableForType() {
            using VTable = std::conditional_t<bRequiresAllocation<T>, TVTableDynamic<T>, TVTableStack<T>>;
            static VTable Table = { VTable::Type, VTable::Destroy, VTable::Copy, VTable::Move, VTable::Swap };
            return &Table;
        }

    protected:

    private:
        FStorageUnion Storage;
        FVTableType *VTable;
    };
}