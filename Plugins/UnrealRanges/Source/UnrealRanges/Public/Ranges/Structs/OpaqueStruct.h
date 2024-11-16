// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Structs.h"
#include "Ranges/Optional/OptionalRef.h"
#include <array>

namespace UE::Ranges {
    constexpr size_t SmallStructSize = sizeof(void*) * 7;

    template <typename T>
    concept SmallStructType = UEStruct<T> && sizeof(T) <= SmallStructSize;

    /**
     * Class FOpaqueStruct manages an opaque script structure, providing various utilities
     * for constructing, copying, moving, and accessing the internal structure.
     */
    class FOpaqueStruct {
    public:
        /**
         * Default constructor for the FOpaqueStruct class.
         * Initializes an instance with no associated script structure.
         *
         * @return A new FOpaqueStruct instance.
         */
        FOpaqueStruct() = default;

        /**
         * Constructs an FOpaqueStruct instance with a specified script structure.
         * This constructor initializes the storage based on the provided structure and ensures
         * efficient handling of memory for the structure.
         *
         * @param Struct The UScriptStruct instance defining the structure to be managed.
         * @return A new FOpaqueStruct instance initialized with the provided structure.
         */
        explicit FOpaqueStruct(const UScriptStruct& Struct) noexcept : Storage(Struct), Struct(&Struct) {}

        /**
         * Constructs an FOpaqueStruct with the given script structure and raw data pointer.
         *
         * @param Struct A reference to the script structure used for constructing the opaque structure.
         * @param Raw A pointer to the raw data associated with the script structure.
         * @return An instance of FOpaqueStruct initialized with the provided script structure and raw data.
         */
        template <typename T>
        explicit FOpaqueStruct(const UScriptStruct& Struct, T* Raw) : Storage(Struct, Raw), Struct(&Struct) {
        }

        /**
         * Constructor FOpaqueStruct initializes the opaque structure with the provided script.
         * Utilizes perfect forwarding to ensure that the structure is handled efficiently.
         *
         * @tparam T Type of the script structure being managed.
         * @param Struct Temporary object of type T, representing the script structure to be stored.
         */
        template <typename T>
            requires UEStruct<T> && (!std::derived_from<std::decay_t<T>, FOpaqueStruct>)
        explicit FOpaqueStruct(T&& Struct) noexcept : Storage(std::forward<T>(Struct)), Struct(GetScriptStruct<T>()) {
        }

        /**
         * Constructs an FOpaqueStruct instance directly in place, utilizing the given arguments to
         * construct either a small or large internal structure based on the type T. Ensures
         * exception safety and forwards arguments optimally.
         *
         * @tparam T The type of the structure to instantiate within the FOpaqueStruct.
         * @tparam A Variadic template parameter pack for the constructor arguments of T.
         * @param Args Constructor arguments for creating an instance of type T.
         */
        template <typename T, typename... A>
            requires UEStruct<T> && std::constructible_from<T, A...>
        explicit FOpaqueStruct(std::in_place_type_t<T>, A &&...Args) noexcept : Struct(GetScriptStruct<T>()) {
            if constexpr (SmallStructType<T>) {
                new (reinterpret_cast<T *>(&Storage.Small)) T(std::forward<A>(Args)...);
            } else {
                Storage.Large = new T(std::forward<A>(Args)...);
            }
        }

        /**
         * Copy constructor for the FOpaqueStruct class.
         * This constructor initializes a new FOpaqueStruct instance by copying another instance, ensuring that the
         * internal storage and associated script structure are duplicated accurately.
         *
         * @param Other The FOpaqueStruct instance to be copied from.
         * @return A new FOpaqueStruct instance initialized with the copied data.
         */
        FOpaqueStruct(const FOpaqueStruct& Other) noexcept : Struct(Other.Struct) {
            Other.Storage.CopyTo(*Struct, Storage);
        }

        /**
         * Move constructor for the FOpaqueStruct class.
         * This constructor initializes a new FOpaqueStruct instance by moving the resources from another instance.
         * It transfers ownership of the internal storage and its associated script structure to the new instance,
         * and sets the original instance's structure pointer to null.
         *
         * @param Other The FOpaqueStruct instance to be moved from.
         * @return A new FOpaqueStruct instance initialized with the moved data.
         */
        FOpaqueStruct(FOpaqueStruct&& Other) noexcept : Struct(Other.Struct)  {
            Other.Storage.MoveTo(*Struct, Storage);
            Other.Struct = nullptr;
        }

        /**
         * Destructor for the FOpaqueStruct class.
         * Cleans up any resources held by the FOpaqueStruct instance.
         * This destructor calls the Reset() method to ensure proper deallocation of the internal storage.
         */
        ~FOpaqueStruct() {
            Reset();
        }

        /**
         * Copy assignment operator for the FOpaqueStruct class.
         * This assignment operator initializes a new FOpaqueStruct instance by copying another instance, ensuring that the
         * internal storage and associated script structure are duplicated accurately.
         *
         * @param Other The FOpaqueStruct instance to be copied from.
         * @return A new FOpaqueStruct instance initialized with the copied data.
         */
        FOpaqueStruct& operator=(const FOpaqueStruct& Other) noexcept {
            Struct = Other.Struct;
            Other.Storage.CopyTo(*Struct, Storage);
            return *this;
        }

        /**
         * Move assignment operator for the FOpaqueStruct class.
         * This assignment operator initializes a new FOpaqueStruct instance by moving the resources from another instance.
         * It transfers ownership of the internal storage and its associated script structure to the new instance,
         * and sets the original instance's structure pointer to null.
         *
         * @param Other The FOpaqueStruct instance to be moved from.
         * @return A new FOpaqueStruct instance initialized with the moved data.
         */
        FOpaqueStruct& operator=(FOpaqueStruct&& Other) noexcept {
            Struct = Other.Struct;
            Other.Storage.MoveTo(*Struct, Storage);
            Other.Struct = nullptr;
            return *this;
        }

        /**
         * Checks if the FOpaqueStruct instance currently holds a valid script structure.
         *
         * @return true if the instance has a script structure; false otherwise.
         */
        bool HasValue() const {
            return Struct != nullptr;
        }

        /**
         * Determines if the FOpaqueStruct instance contains a valid structure of a specified type.
         *
         * @return true if the instance has a script structure and it is a child of the specified type T; false otherwise.
         */
        template <typename T>
            requires UEStruct<T>
        bool IsStruct() const {
            return HasValue() && Struct->IsChildOf<T>();
        }
            
        /**
         * Retrieves the stored structure.
         * Ensures that the structure is an instance of the desired type.
         *
         * @tparam T The desired struct type
         * @return void* A pointer to the structure either from the small or large storage depending
         * on the structure size.
         */
        template <typename T>
            requires UEStruct<T>
        T& Get() {
            check(IsStruct<T>())
            return GetUnchecked<T>();
        }

        /**
         * Retrieves the stored structure.
         * Ensures that the structure is an instance of the desired type.
         *
         * @tparam T The desired struct type
         * @return void* A pointer to the structure either from the small or large storage depending
         * on the structure size.
         */
        template <typename T>
            requires UEStruct<T>
        const T& Get() const {
            check(IsStruct<T>())
            return GetUnchecked<T>();
        }

        /**
         * Retrieves the raw pointer to the stored structure.
         * Ensures that the structure has value before fetching.
         *
         * @return void* A pointer to the structure either from the small or large storage depending
         * on the structure size.
         */
        void* GetRaw() {
            check(HasValue())
            return Struct->GetStructureSize() <= SmallStructSize ? &Storage.Small : Storage.Large;
        }

        /**
         * Retrieves the raw pointer to the stored structure.
         * Ensures that the structure has value before fetching.
         *
         * @return void* A pointer to the structure either from the small or large storage depending
         * on the structure size.
         */
        const void* GetRaw() const {
            check(HasValue())
            return Struct->GetStructureSize() <= SmallStructSize ? &Storage.Small : Storage.Large;
        }

        /**
         * Template function TryGet attempts to retrieve a reference to an object of type T.
         * Returns an optional reference if the type matches; otherwise, returns an empty optional.
         *
         * @tparam T The type to check and retrieve.
         *
         * @return An optional reference to the object of type T if the type matches,
         *         otherwise an empty optional.
         */
        template <typename T>
            requires UEStruct<T>
        TOptional<T&> TryGet() {
            if (!IsStruct<T>()) {
                return TOptional<T&>();
            }
            
            return GetUnchecked<T>();
        }

        /**
         * Template function TryGet attempts to retrieve a reference to an object of type T.
         * Returns an optional reference if the type matches; otherwise, returns an empty optional.
         *
         * @tparam T The type to check and retrieve.
         *
         * @return An optional reference to the object of type T if the type matches,
         *         otherwise an empty optional.
         */
        template <typename T>
            requires UEStruct<T>
        TOptional<const T&> TryGet() const {
            if (!IsStruct<T>()) {
                return TOptional<const T&>();
            }
            
            return GetUnchecked<T>();
        }

        /**
         * Tries to get the raw structure data pointer.
         *
         * @return A TOptional containing the pointer to the raw data if the structure has a value.
         * Returns an empty TOptional if the structure does not have a value.
         */
        TOptional<void> TryGetRaw() {
            if (!HasValue()) {
                return TOptional<void>();
            }
            
            return Struct->GetStructureSize() <= SmallStructSize ? &Storage.Small : Storage.Large;
        }
        
        /**
         * Tries to get the raw structure data pointer.
         *
         * @return A TOptional containing the pointer to the raw data if the structure has a value.
         * Returns an empty TOptional if the structure does not have a value.
         */
        TOptional<const void> TryGetRaw() const {
            if (!HasValue()) {
                return TOptional<const void>();
            }
            
            return Struct->GetStructureSize() <= SmallStructSize ? &Storage.Small : Storage.Large;
        }

        /**
         * Safely resets the internal structure, releasing any associated resources.
         *
         * This method checks if the internal structure (`Struct`) is non-null and
         * then determines the size of the structure. If the structure size is less
         * than or equal to `SmallStructSize`, it destroys the small-sized structure.
         * Otherwise, it destroys and frees the large-sized structure's memory.
         *
         * @note This method is noexcept and guarantees to not throw exceptions.
         */
        void Reset() noexcept {
            if (Struct == nullptr) {
                return;
            }
            
            if (auto Size = static_cast<size_t>(Struct->GetStructureSize()); Size <= SmallStructSize) {
                Struct->DestroyStruct(&Storage.Small);
            } else {
                Struct->DestroyStruct(Storage.Large);
                FMemory::Free(Storage.Large);
            }
        }

    private:
        template <typename T>
        T& GetUnchecked() {
            if (Struct->GetStructureSize() <= SmallStructSize) {
                return *reinterpret_cast<T*>(&Storage.Small);
            }
            
            return *static_cast<T*>(Storage.Large);
        }

        template <typename T>
        const T& GetUnchecked() const {
            if (Struct->GetStructureSize() <= SmallStructSize) {
                return *reinterpret_cast<const T*>(&Storage.Small);
            }
            
            return *static_cast<const T*>(Storage.Large);
        }
        
        union FStorage {
            std::array<std::byte, SmallStructSize> Small;
            void* Large;

            FStorage() : Large(nullptr) {}

            explicit FStorage(const UScriptStruct& Struct) noexcept {
                if (auto Size = static_cast<size_t>(Struct.GetStructureSize()); Size <= SmallStructSize) {
                    Struct.InitializeStruct(&Small);
                } else {
                    Large = FMemory::Malloc(Size);
                    Struct.InitializeStruct(Large);
                }
            }

            template <typename T>
            explicit FStorage(const UScriptStruct& Struct, T* Data) noexcept {
                if (auto Size = static_cast<size_t>(Struct.GetStructureSize()); Size <= SmallStructSize) {
                    Struct.CopyScriptStruct(&Small, Data);
                } else {
                    Large = FMemory::Malloc(Size);
                    Struct.CopyScriptStruct(Large, Data);
                }
            }

            template <typename T>
                requires UEStruct<T>
            explicit FStorage(T&& Struct) noexcept {
                if constexpr (SmallStructType<T>) {
                    new (reinterpret_cast<std::decay_t<T>*>(&Small)) std::decay_t<T>(std::forward<T>(Struct));
                } else {
                    Large = new std::decay_t<T>(std::forward<T>(Struct));
                }
            }

            void CopyTo(const UScriptStruct& StructType, FStorage& Dest) const noexcept {
                if (auto Size = static_cast<size_t>(StructType.GetStructureSize()); Size <= SmallStructSize) {
                    StructType.InitializeStruct(&Dest.Small);
                    StructType.CopyScriptStruct(&Dest.Small, &Small);
                } else {
                    Dest.Large = FMemory::Malloc(Size);
                    StructType.InitializeStruct(Dest.Large);
                    StructType.CopyScriptStruct(Dest.Large, Large);
                }
            }

            void MoveTo(const UScriptStruct& StructType, FStorage& Dest) noexcept {
                if (auto Size = static_cast<size_t>(StructType.GetStructureSize()); Size <= SmallStructSize) {
                    std::memcpy(&Dest.Small, &Small, Size);
                } else {
                    Dest.Large = Large;
                    Large = nullptr;
                }
            }
        };

        
        

        FStorage Storage;
        TObjectPtr<const UScriptStruct> Struct;
    };
}
