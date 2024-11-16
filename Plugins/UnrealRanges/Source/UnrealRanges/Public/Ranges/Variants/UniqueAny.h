// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Optional/OptionalRef.h"
#include <array>

namespace UE::Ranges {
    /**
     * @brief Size of the small buffer used in FUniqueAny for small object optimization.
     *
     * This constant defines the size of the buffer that can be used to store small objects
     * directly within an instance of FUniqueAny, avoiding dynamic memory allocation for such objects.
     */
    static constexpr size_t UniqueAnySmallBufferSize = sizeof(void *) * 7;

    /**
     * Concept that signifies if a piece of data fits inside of the small buffer or not.
     *
     * @tparam T The type to check against
     */
    template <typename T>
    concept FitsInUniqueAnySmallBuffer = sizeof(T) <= UniqueAnySmallBufferSize;

    namespace Details {
        template <typename T>
        struct TIsInPlaceType : std::false_type {};

        template <typename T>
        struct TIsInPlaceType<std::in_place_type_t<T>> : std::true_type {};
    } // namespace Details

    /**
     * @class FUniqueAny
     *
     * @brief A type-safe container for any single value of any type, which ensures unique ownership of the contained
     * value.
     *
     * FUniqueAny allows storing any object, while providing unique ownership semantics (non-copyable, movable).
     * It leverages small object optimization to store small objects directly inside the FUniqueAny instance, and
     * dynamically allocates memory for larger objects.
     */
    class FUniqueAny final {
      public:
        /**
         * @brief Default constructor for FUniqueAny.
         *
         * Initializes an empty FUniqueAny instance. The object constructed with this constructor does not hold any
         * value.
         */
        FUniqueAny() = default;

        /**
         * @brief Constructs an FUniqueAny instance with a given value of any type.
         *
         * This constructor allows for the creation of an FUniqueAny object that holds a value of type T. The value
         * is stored using perfect forwarding to achieve optimal performance.
         *
         * @tparam T The type of the value being stored.
         * @param Value The value to be stored inside the FUniqueAny instance.
         */
        template <typename T>
        explicit FUniqueAny(T &&Value) noexcept
            : Storage(std::forward<T>(Value)), VTable(&GetVTableForType<std::decay_t<T>>()) {
        }

        /**
         * @brief Constructs an FUniqueAny instance in-place with a given type and arguments.
         *
         * This constructor allows for the direct in-place construction of an object of type T within the FUniqueAny
         * instance. It utilizes perfect forwarding to pass arguments Args to the constructor of type T.
         *
         * @tparam T The type of the object to be constructed in-place.
         * @tparam A The types of the arguments to be perfectly forwarded to the constructor of type T.
         * @param in_place_type_t A tag to indicate in-place construction.
         * @param Args The arguments to be perfectly forwarded to the constructor of type T.
         * @return An FUniqueAny instance containing the newly constructed object of type T.
         */
        template <typename T, typename... A>
            requires std::constructible_from<T, A...>
        explicit FUniqueAny(std::in_place_type_t<T>, A &&...Args) noexcept : VTable(&GetVTableForType<T>()) {
            if constexpr (FitsInUniqueAnySmallBuffer<T>) {
                new (reinterpret_cast<T *>(&Storage.SmallStorage)) T(std::forward<A>(Args)...);
            } else {
                Storage.LargeStorage = new T(std::forward<A>(Args)...);
            }
        }

        FUniqueAny(const FUniqueAny &) = delete;

        /**
         * @brief Move constructor for FUniqueAny.
         *
         * Constructs an FUniqueAny instance by transferring the ownership of the value from another FUniqueAny
         * instance. The other instance will be left in an empty state after the move.
         *
         * @param Other The FUniqueAny instance to move from. The state of Other will be invalidated.
         * @return An FUniqueAny instance that takes ownership of the value from the Other instance.
         */
        FUniqueAny(FUniqueAny &&Other) noexcept : VTable(Other.VTable) {
            VTable->Move(Other.Storage, Storage);
            Other.VTable = nullptr;
        }

        /**
         * @brief Destructor for FUniqueAny.
         *
         * Cleans up the stored value, if any, by invoking the appropriate destruction logic through the virtual table.
         * Ensures that the dynamically allocated memory, if any, is properly deallocated.
         */
        ~FUniqueAny() {
            if (VTable != nullptr) {
                VTable->Destroy(Storage);
            }
        }

        FUniqueAny &operator=(const FUniqueAny &) = delete;

        /**
         * @brief Overloads the assignment operator to facilitate the correct transfer of ownership for the contained
         * value.
         *
         * The assignment operator ensures that the value contained in the current instance is properly handled
         * when another instance is assigned to it, maintaining unique ownership semantics.
         *
         * @param Other The right-hand side FUniqueAny instance from which the value is assigned.
         * @return A reference to the current FUniqueAny instance after the assignment.
         */
        FUniqueAny &operator=(FUniqueAny &&Other) noexcept {
            VTable = Other.VTable;
            VTable->Move(Other.Storage, Storage);
            Other.VTable = nullptr;
            return *this;
        }

        /**
         * @brief Overloads the assignment operator for an object of the ExampleClass.
         *
         * This function defines the behavior of the assignment operator,
         * enabling the assignment of one instance of ExampleClass to another.
         *
         * @param Other The instance of ExampleClass to be assigned to this instance.
         * @return A reference to this instance after assignment.
         */
        template <typename T>
        FUniqueAny &operator=(T &&Other) noexcept {
            Emplace<std::decay_t<T>>(std::forward<T>(Other));
            return *this;
        }

        /**
         * @brief Retrieves the stored value of type T, ensuring type safety.
         *
         * This method asserts that the stored value is of the specified type T before
         * returning it. It uses type information to check the type of the stored value
         * and ensures the type matches T. If the type does not match, the function
         * will trigger a runtime check failure.
         *
         * @tparam T The expected type of the stored value.
         * @return A reference to the stored value of type T.
         */
        template <typename T>
        T &Get() {
            check(GetType() == typeid(T))
            return GetUnchecked<T>();
        }

        /**
         * @brief Retrieves the stored value of type T, ensuring type safety.
         *
         * This method asserts that the stored value is of the specified type T before
         * returning it. It uses type information to check the type of the stored value
         * and ensures the type matches T. If the type does not match, the function
         * will trigger a runtime check failure.
         *
         * @tparam T The expected type of the stored value.
         * @return A reference to the stored value of type T.
         */
        template <typename T>
        const T &Get() const {
            check(GetType() == typeid(T))
            return GetUnchecked<T>();
        }

        /**
         * @brief Attempts to retrieve a reference to the stored value if it matches the specified type T.
         *
         * This method checks if the stored value's type matches the specified type T. If it does, it returns a
         * reference to the stored value, wrapped in a TOptional. If the types do not match, it returns an empty
         * TOptional.
         *
         * @return TOptional<T&> containing a reference to the stored value if the type matches, otherwise an empty
         * TOptional.
         */
        template <typename T>
        TOptional<T &> TryGet() {
            if (GetType() != typeid(T)) {
                return TOptional<T &>();
            }

            return GetUnchecked<T>();
        }

        /**
         * @brief Attempts to retrieve a reference to the stored value if it matches the specified type T.
         *
         * This method checks if the stored value's type matches the specified type T. If it does, it returns a
         * reference to the stored value, wrapped in a TOptional. If the types do not match, it returns an empty
         * TOptional.
         *
         * @return TOptional<T&> containing a reference to the stored value if the type matches, otherwise an empty
         * TOptional.
         */
        template <typename T>
        TOptional<const T &> TryGet() const {
            if (GetType() != typeid(T)) {
                return TOptional<const T &>();
            }

            return GetUnchecked<T>();
        }

        /**
         * @brief Emplaces a new object of type T using the provided arguments, destroying the currently stored object
         * if nedded.
         *
         * @tparam T The type of object to store
         * @tparam A The types of the constructor arguments
         * @param Args Arguments to construct the new object with.
         */
        template <typename T, typename... A>
        void Emplace(A &&...Args) {
            if (HasValue()) {
                VTable->Destroy(Storage);
            }

            if constexpr (FitsInUniqueAnySmallBuffer<std::decay_t<T>>) {
                new (reinterpret_cast<T *>(&Storage.SmallStorage)) std::decay_t<T>(std::forward<A>(Args)...);
            } else {
                Storage.LargeStorage = new std::decay_t<T>(std::forward<A>(Args)...);
            }
            VTable = &GetVTableForType<std::decay_t<T>>();
        }

        /**
         *  @brief Resets the contained value, if it exists, by destroying it and setting internal state to null.
         *
         *  This method checks if a value is currently held using `HasValue()`. If a value exists,
         *  it invokes the `Destroy` method from the virtual table pointer (`VTable`) to clean up
         *  the stored object, and then sets the `VTable` pointer to `nullptr`, effectively resetting the state.
         */
        void Reset() noexcept {
            if (HasValue()) {
                VTable->Destroy(Storage);
                VTable = nullptr;
            }
        }

        /**
         * @brief Checks if the object currently holds a value.
         *
         * Determines whether the object has an associated value by checking if
         * the internal VTable pointer is non-null.
         *
         * @return True if the object holds a value, false otherwise.
         */
        bool HasValue() const {
            return VTable != nullptr;
        }

        /**
         * @brief Retrieves the type information of the stored value.
         *
         * This method returns the type information of the stored value if one exists;
         * otherwise, it returns the type information for void.
         *
         * @return The type information of the stored value or void if no value is present.
         */
        const std::type_info &GetType() const noexcept {
            return HasValue() ? VTable->GetType() : typeid(void);
        }

      private:
        template <typename T>
        constexpr T &GetUnchecked() {
            if constexpr (FitsInUniqueAnySmallBuffer<T>) {
                return reinterpret_cast<T &>(Storage.SmallStorage);
            } else {
                return *static_cast<T *>(Storage.LargeStorage);
            }
        }

        template <typename T>
        constexpr const T &GetUnchecked() const {
            if constexpr (FitsInUniqueAnySmallBuffer<T>) {
                return reinterpret_cast<const T &>(Storage.SmallStorage);
            } else {
                return *static_cast<const T *>(Storage.LargeStorage);
            }
        }

        union FStorage {
            std::array<std::byte, UniqueAnySmallBufferSize> SmallStorage;
            void *LargeStorage;

            FStorage() : LargeStorage(nullptr) {
            }

            template <typename T>
                requires FitsInUniqueAnySmallBuffer<std::decay_t<T>>
            explicit FStorage(T &&Data) noexcept {
                new (reinterpret_cast<std::decay_t<T> *>(&SmallStorage)) std::decay_t<T>(std::forward<T>(Data));
            }

            template <typename T>
                requires(!FitsInUniqueAnySmallBuffer<std::decay_t<T>>)
            explicit FStorage(T &&Data) noexcept : LargeStorage(new std::decay_t<T>(std::forward<T>(Data))) {
            }

            template <typename T>
                requires(!FitsInUniqueAnySmallBuffer<std::decay_t<T>>)
            explicit FStorage(T *Data) noexcept : LargeStorage(Data) {
            }
        };

        struct FVTable {
            const std::type_info &(*GetType)() noexcept;
            void (*Destroy)(FStorage &Storage);
            void (*Move)(FStorage &Source, FStorage &Dest) noexcept;
        };

        template <typename T>
        struct TVTableImpl {
            static const std::type_info &GetType() noexcept {
                return typeid(T);
            }

            static void Destroy(FStorage &Storage) noexcept {
                if constexpr (FitsInUniqueAnySmallBuffer<T>) {
                    reinterpret_cast<T *>(&Storage.SmallStorage)->~T();
                } else {
                    delete static_cast<T *>(Storage.LargeStorage);
                }
            }

            static void Move(FStorage &Source, FStorage &Dest) noexcept {
                if constexpr (FitsInUniqueAnySmallBuffer<T>) {
                    std::memcpy(&Dest.SmallStorage, &Source.SmallStorage, sizeof(T));
                } else {
                    Dest.LargeStorage = Source.LargeStorage;
                    Source.LargeStorage = nullptr;
                }
            }
        };

        template <typename T>
        static FVTable &GetVTableForType() {
            // clang-fromat off
            static FVTable VTable = {
                .GetType = TVTableImpl<T>::GetType,
                .Destroy = TVTableImpl<T>::Destroy,
                .Move = TVTableImpl<T>::Move
            };
            // clang-format on
            return VTable;
        }

        FStorage Storage;
        FVTable *VTable = nullptr;
    };
} // namespace UE::Ranges
