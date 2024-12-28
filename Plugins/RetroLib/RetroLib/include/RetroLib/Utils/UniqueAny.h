/**
 * @file UniqueAny.h
 * @brief Move-only version of std::any.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Concepts/Inheritance.h"
#include "RetroLib/Concepts/OpaqueStorage.h"
#include "RetroLib/Optionals/Optional.h"

#include <any>
#include <array>
#include <bit>
#include <typeinfo>
#include <utility>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro {

    template <typename T>
    concept FitsInUniqueAnySmallBuffer = sizeof(T) <= DEFAULT_SMALL_STORAGE_SIZE;

    /**
     * @class UniqueAny
     *
     * @brief A type-safe container for any single value of any type, which ensures unique ownership of the contained
     * value.
     *
     * UniqueAny allows storing any object, while providing unique ownership semantics (non-copyable, movable).
     * It leverages small object optimization to store small objects directly inside the UniqueAny instance, and
     * dynamically allocates memory for larger objects.
     */
    RETROLIB_EXPORT class UniqueAny final {
      public:
        /**
         * @brief Default constructor for UniqueAny.
         *
         * Initializes an empty UniqueAny instance. The object constructed with this constructor does not hold any
         * value.
         */
        UniqueAny() = default;

        /**
         * @brief Constructs an UniqueAny instance with a given value of any type.
         *
         * This constructor allows for the creation of an UniqueAny object that holds a value of type T. The value
         * is stored using perfect forwarding to achieve optimal performance.
         *
         * @tparam T The type of the value being stored.
         * @param Value The value to be stored inside the UniqueAny instance.
         */
        template <typename T>
            requires(!std::same_as<std::decay_t<T>, UniqueAny>)
        explicit(false) UniqueAny(T &&Value) noexcept
            : storage(std::forward<T>(Value)), vtable(&get_vtable_for_type<std::decay_t<T>>()) {
        }

        /**
         * @brief Constructs an UniqueAny instance in-place with a given type and arguments.
         *
         * This constructor allows for the direct in-place construction of an object of type T within the UniqueAny
         * instance. It utilizes perfect forwarding to pass arguments Args to the constructor of type T.
         *
         * @tparam T The type of the object to be constructed in-place.
         * @tparam A The types of the arguments to be perfectly forwarded to the constructor of type T.
         * @param in_place_type_t A tag to indicate in-place construction.
         * @param Args The arguments to be perfectly forwarded to the constructor of type T.
         * @return An UniqueAny instance containing the newly constructed object of type T.
         */
        template <typename T, typename... A>
            requires std::constructible_from<T, A...>
        explicit UniqueAny(std::in_place_type_t<T>, A &&...Args) noexcept : vtable(&get_vtable_for_type<T>()) {
            if constexpr (FitsInUniqueAnySmallBuffer<T>) {
                new (std::bit_cast<T *>(storage.small_storage.data())) T(std::forward<A>(Args)...);
            } else {
                storage.large_storage = new T(std::forward<A>(Args)...);
            }
        }

        UniqueAny(const UniqueAny &) = delete;

        /**
         * @brief Move constructor for UniqueAny.
         *
         * Constructs an UniqueAny instance by transferring the ownership of the value from another UniqueAny
         * instance. The other instance will be left in an empty state after the move.
         *
         * @param Other The UniqueAny instance to move from. The state of Other will be invalidated.
         * @return An UniqueAny instance that takes ownership of the value from the Other instance.
         */
        UniqueAny(UniqueAny &&Other) noexcept : vtable(Other.vtable) {
            vtable->move(Other.storage, storage);
            Other.vtable = nullptr;
        }

        /**
         * @brief Destructor for UniqueAny.
         *
         * Cleans up the stored value, if any, by invoking the appropriate destruction logic through the virtual table.
         * Ensures that the dynamically allocated memory, if any, is properly deallocated.
         */
        ~UniqueAny() {
            if (vtable != nullptr) {
                vtable->destroy(storage);
            }
        }

        UniqueAny &operator=(const UniqueAny &) = delete;

        /**
         * @brief Overloads the assignment operator to facilitate the correct transfer of ownership for the contained
         * value.
         *
         * The assignment operator ensures that the value contained in the current instance is properly handled
         * when another instance is assigned to it, maintaining unique ownership semantics.
         *
         * @param Other The right-hand side UniqueAny instance from which the value is assigned.
         * @return A reference to the current UniqueAny instance after the assignment.
         */
        UniqueAny &operator=(UniqueAny &&Other) noexcept {
            vtable = Other.vtable;
            vtable->move(Other.storage, storage);
            Other.vtable = nullptr;
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
        UniqueAny &operator=(T &&Other) noexcept {
            emplace<std::decay_t<T>>(std::forward<T>(Other));
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
        T &get() {
            if (get_type() != typeid(T)) {
                throw std::bad_any_cast();
            }

            return get_unchecked<T>();
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
        const T &get() const {
            if (get_type() != typeid(T)) {
                throw std::bad_any_cast();
            }

            return get_unchecked<T>();
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
        Optional<T &> try_get() {
            if (get_type() != typeid(T)) {
                return std::nullopt;
            }

            return get_unchecked<T>();
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
        Optional<const T &> try_get() const {
            if (get_type() != typeid(T)) {
                return Optional<const T &>();
            }

            return get_unchecked<T>();
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
        void emplace(A &&...Args) {
            if (has_value()) {
                vtable->destroy(storage);
            }

            if constexpr (FitsInUniqueAnySmallBuffer<std::decay_t<T>>) {
                new (std::bit_cast<T *>(storage.small_storage.data())) std::decay_t<T>(std::forward<A>(Args)...);
            } else {
                storage.large_storage = new std::decay_t<T>(std::forward<A>(Args)...);
            }
            vtable = &get_vtable_for_type<std::decay_t<T>>();
        }

        /**
         *  @brief Resets the contained value, if it exists, by destroying it and setting internal state to null.
         *
         *  This method checks if a value is currently held using `HasValue()`. If a value exists,
         *  it invokes the `Destroy` method from the virtual table pointer (`VTable`) to clean up
         *  the stored object, and then sets the `VTable` pointer to `nullptr`, effectively resetting the state.
         */
        void reset() noexcept {
            if (has_value()) {
                vtable->destroy(storage);
                vtable = nullptr;
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
        bool has_value() const {
            return vtable != nullptr;
        }

        /**
         * @brief Retrieves the type information of the stored value.
         *
         * This method returns the type information of the stored value if one exists;
         * otherwise, it returns the type information for void.
         *
         * @return The type information of the stored value or void if no value is present.
         */
        const std::type_info &get_type() const noexcept {
            return has_value() ? *vtable->type : typeid(void);
        }

      private:
        template <typename T>
        constexpr T &get_unchecked() {
            if constexpr (FitsInUniqueAnySmallBuffer<T>) {
                return *std::bit_cast<T *>(storage.small_storage.data());
            } else {
                return *static_cast<T *>(storage.large_storage);
            }
        }

        template <typename T>
        constexpr const T &get_unchecked() const {
            if constexpr (FitsInUniqueAnySmallBuffer<T>) {
                return *std::bit_cast<const T *>(storage.small_storage.data());
            } else {
                return *static_cast<const T *>(storage.large_storage);
            }
        }

        union Storage {
            std::array<std::byte, DEFAULT_SMALL_STORAGE_SIZE> small_storage;
            void *large_storage;

            Storage() : large_storage(nullptr) {
            }

            template <typename T>
                requires FitsInUniqueAnySmallBuffer<std::decay_t<T>> && (!std::same_as<std::decay_t<T>, Storage>)
            explicit Storage(T &&Data) noexcept {
                new (std::bit_cast<std::decay_t<T> *>(small_storage.data())) std::decay_t<T>(std::forward<T>(Data));
            }

            template <typename T>
                requires(!FitsInUniqueAnySmallBuffer<std::decay_t<T>>) && (!std::same_as<std::decay_t<T>, Storage>)
            explicit Storage(T &&Data) noexcept : large_storage(new std::decay_t<T>(std::forward<T>(Data))) {
            }

            template <typename T>
                requires(!FitsInUniqueAnySmallBuffer<std::decay_t<T>>)
            explicit Storage(T *Data) noexcept : large_storage(Data) {
            }
        };

        struct VTable {
            const std::type_info *type;
            bool is_large = false;
            void (*destroy)(Storage &Storage);
            void (*move)(Storage &Source, Storage &Dest) noexcept;
        };

        template <typename T>
        struct VTableImpl {
            static void destroy(Storage &Storage) noexcept {
                if constexpr (FitsInUniqueAnySmallBuffer<T>) {
                    std::bit_cast<T *>(Storage.small_storage.data())->~T();
                } else {
                    delete static_cast<T *>(Storage.large_storage);
                }
            }

            static void move(Storage &Source, Storage &Dest) noexcept {
                if constexpr (FitsInUniqueAnySmallBuffer<T>) {
                    std::memcpy(&Dest.small_storage, &Source.small_storage, sizeof(T));
                } else {
                    Dest.large_storage = Source.large_storage;
                    Source.large_storage = nullptr;
                }
            }
        };

        template <typename T>
        static VTable &get_vtable_for_type() {
            // clang-format off
            static VTable vtable = {
                .type = &typeid(T),
                .is_large = !FitsInUniqueAnySmallBuffer<T>,
                .destroy = VTableImpl<T>::destroy,
                .move = VTableImpl<T>::move
            };
            // clang-format on
            return vtable;
        }

        Storage storage;
        VTable *vtable = nullptr;
    };

} // namespace retro