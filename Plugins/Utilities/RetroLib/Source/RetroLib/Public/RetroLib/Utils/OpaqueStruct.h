// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#ifdef __UNREAL__
#include "RetroLib/Concepts/OpaqueStorage.h"
#include "RetroLib/Concepts/Structs.h"
#include "RetroLib/Optionals/OptionalOperations.h"
#include <array>

namespace Retro {
    template <typename T>
    concept SmallStructType = UEStruct<T> && sizeof(T) <= DEFAULT_SMALL_STORAGE_SIZE;

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
        explicit FOpaqueStruct(const UScriptStruct &Struct) noexcept : Storage(Struct), Struct(&Struct) {
        }

        /**
         * Constructs an FOpaqueStruct with the given script structure and raw data pointer.
         *
         * @param Struct A reference to the script structure used for constructing the opaque structure.
         * @param Raw A pointer to the raw data associated with the script structure.
         * @return An instance of FOpaqueStruct initialized with the provided script structure and raw data.
         */
        template <typename T>
        explicit FOpaqueStruct(const UScriptStruct &Struct, T *Raw) : Storage(Struct, Raw), Struct(&Struct) {
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
        explicit FOpaqueStruct(T &&Struct) noexcept : Storage(std::forward<T>(Struct)), Struct(GetScriptStruct<T>()) {
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
        FOpaqueStruct(const FOpaqueStruct &Other) noexcept : Struct(Other.Struct) {
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
        FOpaqueStruct(FOpaqueStruct &&Other) noexcept : Struct(Other.Struct) {
            Other.Storage.MoveTo(*Struct, Storage);
            Other.Struct = nullptr;
        }

        /**
         * Destructor for the FOpaqueStruct class.
         * Cleans up any resources held by the FOpaqueStruct instance.
         * This destructor calls the Reset() method to ensure proper deallocation of the internal storage.
         */
        ~FOpaqueStruct() {
            if (Struct != nullptr) {
                Storage.Destroy(*Struct);
            }
        }

        /**
         * Copy assignment operator for the FOpaqueStruct class.
         * This assignment operator initializes a new FOpaqueStruct instance by copying another instance, ensuring that
         * the internal storage and associated script structure are duplicated accurately.
         *
         * @param Other The FOpaqueStruct instance to be copied from.
         * @return A new FOpaqueStruct instance initialized with the copied data.
         */
        FOpaqueStruct &operator=(const FOpaqueStruct &Other) noexcept {
            if (Struct != nullptr) {
                Storage.Destroy(*Struct);
            }

            Struct = Other.Struct;
            Other.Storage.CopyTo(*Struct, Storage);
            return *this;
        }

        /**
         * Move assignment operator for the FOpaqueStruct class.
         * This assignment operator initializes a new FOpaqueStruct instance by moving the resources from another
         * instance. It transfers ownership of the internal storage and its associated script structure to the new
         * instance, and sets the original instance's structure pointer to null.
         *
         * @param Other The FOpaqueStruct instance to be moved from.
         * @return A new FOpaqueStruct instance initialized with the moved data.
         */
        FOpaqueStruct &operator=(FOpaqueStruct &&Other) noexcept {
            if (Struct != nullptr) {
                Storage.Destroy(*Struct);
            }

            Struct = Other.Struct;
            Other.Storage.MoveTo(*Struct, Storage);
            Other.Struct = nullptr;
            return *this;
        }

        /**
         * Assigns a new value to the FOpaqueStruct from a given data object.
         *
         * @tparam T The type of the data object being assigned.
         * @param Data The data object to be assigned to the structure. Could be of either small or large struct type.
         * @return Reference to the updated FOpaqueStruct.
         *
         * This method performs a type check at compile-time to ensure the appropriate storage
         * is selected. If the FOpaqueStruct already contains a structure, the current structure
         * will be destroyed before assigning the new data.
         *
         * The method is marked noexcept to indicate it does not throw exceptions.
         */
        template <typename T>
            requires UEStruct<T>
        FOpaqueStruct &operator=(T &&Data) noexcept {
            if (Struct != nullptr) {
                Storage.Destroy(*Struct);
            }

            Struct = GetScriptStruct<T>();
            if constexpr (SmallStructType<T>) {
                new (reinterpret_cast<std::decay_t<T> *>(&Storage.Small)) std::decay_t<T>(std::forward<T>(Data));
            } else {
                Storage.Large = new std::decay_t<T>(std::forward<T>(Data));
            }
            return *this;
        }

        /**
         * Emplaces a new script structure into the storage, replacing any existing structure.
         *
         * @param StructType The script structure type to be emplaced.
         *
         * @note If a structure already exists, it will be destroyed before the new one is emplaced.
         *       The new structure is initialized in the storage, either using small or large allocation
         *       based on the size of the structure.
         */
        void Emplace(const UScriptStruct &StructType) noexcept {
            if (Struct != nullptr) {
                Storage.Destroy(*Struct);
            }

            Struct = &StructType;
            if (auto Size = static_cast<size_t>(Struct->GetStructureSize()); Size <= DEFAULT_SMALL_STORAGE_SIZE) {
                Struct->InitializeStruct(&Storage.Small);
            } else {
                Storage.Large = FMemory::Malloc(Size);
                Struct->InitializeStruct(Storage.Large);
            }
        }

        /**
         * Places a new structure of the given type into the storage. The method manages
         * the existing structure by first destroying it if it exists, and then placing
         * the new structure by allocating memory accordingly.
         *
         * @param StructType The type of the script structure to be placed.
         * @param Data The data for the script structure to be placed.
         */
        template <typename T>
        void Emplace(const UScriptStruct &StructType, T *Data) noexcept {
            if (Struct != nullptr) {
                Storage.Destroy(*Struct);
            }

            Struct = &StructType;
            if (auto Size = static_cast<size_t>(Struct->GetStructureSize()); Size <= DEFAULT_SMALL_STORAGE_SIZE) {
                Struct->InitializeStruct(&Storage.Small);
                Struct->CopyScriptStruct(&Storage.Small, Data);
            } else {
                Storage.Large = FMemory::Malloc(Size);
                Struct->InitializeStruct(Storage.Large);
                Struct->CopyScriptStruct(Storage.Large, Data);
            }
        }

        /**
         * Emplaces a new instance of T within the internal storage, destroying any existing instance.
         *
         * @param Args Variadic arguments used to construct the new instance of T.
         */
        template <typename T, typename... A>
            requires UEStruct<T>
        void Emplace(A &&...Args) noexcept {
            if (Struct != nullptr) {
                Storage.Destroy(*Struct);
            }

            Struct = GetScriptStruct<T>();
            if constexpr (SmallStructType<T>) {
                new (reinterpret_cast<T *>(&Storage.Small)) T(std::forward<A>(Args)...);
            } else {
                Storage.Large = new T(std::forward<A>(Args)...);
            }
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
         * @return true if the instance has a script structure and it is a child of the specified type T; false
         * otherwise.
         */
        template <typename T>
            requires UEStruct<T>
        bool IsStruct() const {
            if constexpr (TModels_V<CStaticStructProvider, T>) {
                return HasValue() && Struct->IsChildOf<T>();
            } else {
                return HasValue() && Struct == GetScriptStruct<T>();
            }
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
        T &Get() {
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
        const T &Get() const {
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
        void *GetRaw() {
            check(HasValue())
            return Struct->GetStructureSize() <= DEFAULT_SMALL_STORAGE_SIZE ? &Storage.Small : Storage.Large;
        }

        /**
         * Retrieves the raw pointer to the stored structure.
         * Ensures that the structure has value before fetching.
         *
         * @return void* A pointer to the structure either from the small or large storage depending
         * on the structure size.
         */
        const void *GetRaw() const {
            check(HasValue())
            return Struct->GetStructureSize() <= DEFAULT_SMALL_STORAGE_SIZE ? &Storage.Small : Storage.Large;
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
        TOptional<T &> TryGet() {
            if (!IsStruct<T>()) {
                return TOptional<T &>();
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
        TOptional<const T &> TryGet() const {
            if (!IsStruct<T>()) {
                return TOptional<const T &>();
            }

            return GetUnchecked<T>();
        }

        /**
         * Tries to get the raw structure data pointer.
         *
         * @return A TOptional containing the pointer to the raw data if the structure has a value.
         * Returns an empty TOptional if the structure does not have a value.
         */
        TOptional<void *> TryGetRaw() {
            if (!HasValue()) {
                return TOptional<void *>();
            }

            return Struct->GetStructureSize() <= DEFAULT_SMALL_STORAGE_SIZE ? &Storage.Small : Storage.Large;
        }

        /**
         * Tries to get the raw structure data pointer.
         *
         * @return A TOptional containing the pointer to the raw data if the structure has a value.
         * Returns an empty TOptional if the structure does not have a value.
         */
        TOptional<const void *> TryGetRaw() const {
            if (!HasValue()) {
                return TOptional<const void *>();
            }

            return Struct->GetStructureSize() <= DEFAULT_SMALL_STORAGE_SIZE ? &Storage.Small : Storage.Large;
        }

        /**
         * Safely resets the internal structure, releasing any associated resources.
         *
         * This method checks if the internal structure (`Struct`) is non-null and
         * then determines the size of the structure. If the structure size is less
         * than or equal to `DEFAULT_SMALL_STORAGE_SIZE`, it destroys the small-sized structure.
         * Otherwise, it destroys and frees the large-sized structure's memory.
         *
         * @note This method is noexcept and guarantees to not throw exceptions.
         */
        void Reset() noexcept {
            if (Struct == nullptr) {
                return;
            }

            Storage.Destroy(*Struct);
            Struct = nullptr;
        }

      private:
        template <typename T>
        T &GetUnchecked() {
            if (Struct->GetStructureSize() <= DEFAULT_SMALL_STORAGE_SIZE) {
                return *reinterpret_cast<T *>(&Storage.Small);
            }

            return *static_cast<T *>(Storage.Large);
        }

        template <typename T>
        const T &GetUnchecked() const {
            if (Struct->GetStructureSize() <= DEFAULT_SMALL_STORAGE_SIZE) {
                return *reinterpret_cast<const T *>(&Storage.Small);
            }

            return *static_cast<const T *>(Storage.Large);
        }

        union FStorage {
            std::array<std::byte, DEFAULT_SMALL_STORAGE_SIZE> Small;
            void *Large;

            FStorage() : Large(nullptr) {
            }

            explicit FStorage(const UScriptStruct &Struct) noexcept {
                if (auto Size = static_cast<size_t>(Struct.GetStructureSize()); Size <= DEFAULT_SMALL_STORAGE_SIZE) {
                    Struct.InitializeStruct(&Small);
                } else {
                    Large = FMemory::Malloc(Size);
                    Struct.InitializeStruct(Large);
                }
            }

            template <typename T>
            explicit FStorage(const UScriptStruct &Struct, T *Data) noexcept {
                if (auto Size = static_cast<size_t>(Struct.GetStructureSize()); Size <= DEFAULT_SMALL_STORAGE_SIZE) {
                    Struct.CopyScriptStruct(&Small, Data);
                } else {
                    Large = FMemory::Malloc(Size);
                    Struct.CopyScriptStruct(Large, Data);
                }
            }

            template <typename T>
                requires UEStruct<T>
            explicit FStorage(T &&Struct) noexcept {
                if constexpr (SmallStructType<T>) {
                    new (reinterpret_cast<std::decay_t<T> *>(&Small)) std::decay_t<T>(std::forward<T>(Struct));
                } else {
                    Large = new std::decay_t<T>(std::forward<T>(Struct));
                }
            }

            void Destroy(const UScriptStruct &StructType) noexcept {
                if (auto Size = static_cast<size_t>(StructType.GetStructureSize());
                    Size <= DEFAULT_SMALL_STORAGE_SIZE) {
                    StructType.DestroyStruct(&Small);
                } else {
                    StructType.DestroyStruct(Large);
                    FMemory::Free(Large);
                }
            }

            void CopyTo(const UScriptStruct &StructType, FStorage &Dest) const noexcept {
                if (auto Size = static_cast<size_t>(StructType.GetStructureSize());
                    Size <= DEFAULT_SMALL_STORAGE_SIZE) {
                    StructType.InitializeStruct(&Dest.Small);
                    StructType.CopyScriptStruct(&Dest.Small, &Small);
                } else {
                    Dest.Large = FMemory::Malloc(Size);
                    StructType.InitializeStruct(Dest.Large);
                    StructType.CopyScriptStruct(Dest.Large, Large);
                }
            }

            void MoveTo(const UScriptStruct &StructType, FStorage &Dest) noexcept {
                if (auto Size = static_cast<size_t>(StructType.GetStructureSize());
                    Size <= DEFAULT_SMALL_STORAGE_SIZE) {
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
} // namespace Retro
#endif