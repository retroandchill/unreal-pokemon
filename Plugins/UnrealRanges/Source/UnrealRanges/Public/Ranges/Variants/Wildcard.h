// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Functional/FunctionPointers.h"
#include "Ranges/Optional/OptionalRef.h"
#include <array>
#include <any>

namespace UE::Ranges {
    class FWildcard final {
    public:
        constexpr FWildcard() = default;

        TArray<uint8> Data;

        template <typename T>
            requires (!std::same_as<std::decay_t<T>, FWildcard>)
        explicit(false) constexpr FWildcard(T &&Value) : Storage(CreateStorage(std::forward<T>(Value))), VTable(VTableForType<std::decay_t<T>>()) {
        }

        FWildcard(const FWildcard & Other) : VTable(Other.VTable) {
            if (Other.HasValue()) {
                Other.VTable->Copy(Other.Storage, Storage);
            }
        }

        FWildcard(FWildcard && Other) : VTable(Other.VTable) {
            if (Other.HasValue()) {
                Other.VTable->Move(Other.Storage, Storage);
            }
        }

        ~FWildcard() {
            Reset();
        }

        FWildcard & operator=(const FWildcard & Other) {
            if (Other.HasValue()) {
                Other.VTable->Copy(Other.Storage, Storage);
            }
            VTable = Other.VTable;
            return *this;
        }

        FWildcard & operator=(FWildcard && Other) {
            if (Other.HasValue()) {
                Other.VTable->Move(Other.Storage, Storage);
            }
            VTable = Other.VTable;
            return *this;
            
        }

        template <typename T>
            requires (!std::same_as<T, void>)
        constexpr T& Get() {
            check(IsType<T>())
            if constexpr (bRequiresAllocation<T>) {
                return *static_cast<T *>(Storage.Dynamic);
            } else {
                return *reinterpret_cast<T *>(&Storage.Stack);
            }
        }
        
        template <typename T>
            requires (!std::same_as<T, void>)
        constexpr const T& Get() const {
            check(IsType<T>())
            if constexpr (bRequiresAllocation<T>) {
                return *static_cast<const T *>(Storage.Dynamic);
            } else {
                return *reinterpret_cast<const T *>(&Storage.Stack);
            }
        }
        
        template <typename T>
            requires (!std::same_as<T, void>)
        constexpr TOptional<T&> TryGet() {
            if (!IsType<T>()) {
                return nullptr;
            }
            
            return &Get<T>();
        }
        
        template <typename T>
            requires (!std::same_as<T, void>)
        constexpr TOptional<const T&> TryGet() const {
            if (!IsType<T>()) {
                return nullptr;
            }
            
            return &Get<T>();
        }

        template <typename T>
            requires (!std::same_as<T, void>)
        constexpr bool IsType() const {
            return GetType() == typeid(T);
        }
        

        constexpr bool HasValue() const noexcept {
            return VTable != nullptr;
        }

        constexpr void Reset() noexcept {
            if (HasValue()) {
                VTable->Destroy(Storage);
                VTable = nullptr;
            }
        }

        constexpr const std::type_info & GetType() const noexcept {
            return HasValue() ? VTable->Type() : typeid(void);
        }

        void Swap(FWildcard & Other) noexcept {
            if (VTable != Other.VTable) {
                FWildcard Temp(std::move(Other));

                Other.VTable = VTable;
                if (VTable != nullptr) {
                    VTable->Move(Storage, Other.Storage);
                }

                VTable = Temp.VTable;
                if (Temp.VTable != nullptr) {
                    Temp.VTable->Move(Temp.Storage, Storage);
                    Temp.VTable = nullptr;
                }
            } else {
                if (VTable != nullptr) {
                    VTable->Swap(Storage, Other.Storage);
                }
            }
        }

    private:
        union FStorageUnion {
            static constexpr size_t SmallStorageSize = sizeof(void *) * 2;
            using FStackStorage = std::array<std::byte, SmallStorageSize>;

            void *Dynamic;
            FStackStorage Stack;
        };

        struct FVTableType {
            TFunctionPtr<const std::type_info&() noexcept> Type;
            TFunctionPtr<void(FStorageUnion &) noexcept> Destroy;
            TFunctionPtr<void(const FStorageUnion &Src, FStorageUnion &Dest)> Copy;
            TFunctionPtr<void(FStorageUnion &Src, FStorageUnion &Dest) noexcept> Move;
            TFunctionPtr<void(FStorageUnion &Src, FStorageUnion &Dest) noexcept> Swap;
        };

        template <typename T>
        struct TVTableDynamic {
            static constexpr const std::type_info &Type() noexcept {
                return typeid(T);
            }

            static constexpr void Destroy(FStorageUnion &Storage) noexcept {
                delete static_cast<T *>(Storage.Dynamic);
            }

            static constexpr void Copy(const FStorageUnion &Src, FStorageUnion &Dest) {
                Dest.Dynamic = new T(*static_cast<const T *>(Src.Dynamic));
            }

            static constexpr void Move(FStorageUnion &Src, FStorageUnion &Dest) noexcept {
                Dest.Dynamic = Src.Dynamic;
                Src.Dynamic = nullptr;
            }

            static constexpr void Swap(FStorageUnion &LHS, FStorageUnion &RHS) noexcept {
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
            requires (!std::same_as<std::decay_t<T>, FWildcard>)
        static constexpr FStorageUnion CreateStorage(T &&Value) {
            FStorageUnion Storage;
            if constexpr (bRequiresAllocation<std::decay_t<T>>) {
                *reinterpret_cast<std::decay_t<T> *>(&Storage.Stack) = std::forward<T>(Value);
            } else {
                Storage.Dynamic = new T(std::forward<T>(Value));
            }
            return Storage;
        }
            

        template <typename T>
        static constexpr FVTableType* VTableForType() {
            using VTable = std::conditional_t<bRequiresAllocation<T>, TVTableDynamic<T>, TVTableStack<T>>;
            static VTable Table = { &VTable::Type, &VTable::Destroy, &VTable::Copy, &VTable::Move, &VTable::Swap };
            return &Table;
        }
        
        FStorageUnion Storage;
        FVTableType *VTable = nullptr;
    };
}