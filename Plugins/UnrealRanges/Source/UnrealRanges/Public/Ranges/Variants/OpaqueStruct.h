// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Structs.h"
#include "Ranges/Optional/OptionalRef.h"
#include "Ranges/Pointers/StructDeleter.h"

namespace UE::Ranges {

    /**
     * @struct FOpaqueStruct
     * @brief Represents an opaque data structure with private implementation details.
     *
     * This structure serves as an abstraction layer that hides the specific details
     * of the underlying data. It is used in scenarios where encapsulation and data
     * hiding are necessary. Custom operations and functionalities can be provided
     * through accompanying functions that operate on FOpaqueStruct pointers.
     */
    class UNREALRANGES_API FOpaqueStruct {
        using FStorage = std::unique_ptr<void, FStructDeleter>;

      public:
        /**
         * Default constructor for FOpaqueStruct.
         *
         * This constructor initializes an instance of FOpaqueStruct with default settings.
         * No underlying data or structure is associated with the instance initially.
         * Useful for creating an object with no data that can be assigned or modified later.
         *
         * @return A default-initialized FOpaqueStruct object.
         */
        FOpaqueStruct() = default;

        /**
         * Constructs an FOpaqueStruct instance using the specified UScriptStruct.
         *
         * This constructor initializes the FOpaqueStruct with storage created based
         * on the provided UScriptStruct. It is used to encapsulate the structure,
         * enabling operations and functionalities to be performed on it while
         * hiding its specific implementation details.
         *
         * @param Struct A pointer to the UScriptStruct that this FOpaqueStruct will encapsulate.
         * @return An instance of FOpaqueStruct initialized with the given UScriptStruct.
         */
        explicit FOpaqueStruct(const UScriptStruct *Struct) noexcept;

        /**
         * @brief Constructs an FOpaqueStruct with the given initialization.
         *
         * This constructor initializes the FOpaqueStruct using a known type through
         * perfect forwarding.
         *
         * @tparam T The type of the structure being forwarded.
         * @param Struct The structure to initialize the FOpaqueStruct with.
         */
        template <typename T>
            requires UEStruct<T>
        explicit FOpaqueStruct(T &&Struct) noexcept : Storage(InitializeFromKnown<T>(std::forward<T>(Struct))) {
        }

        /**
         * @brief Constructs an FOpaqueStruct with the given data and script struct.
         *
         * Initializes the FOpaqueStruct by copying data from an unknown type.
         *
         * @tparam T The type of the data pointer
         * @param Data Pointer to the data of type T.
         * @param Struct Pointer to the UScriptStruct describing the data structure.
         * @return FOpaqueStruct instance initialized with the provided data and struct information.
         */
        template <typename T>
        FOpaqueStruct(const T *Data, const UScriptStruct *Struct) noexcept : Storage(CopyFromUnknown<T>(Data, Struct)) {
        }

        /**
         * Default destructor for FOpaqueStruct.
         *
         * This destructor ensures proper cleanup of any resources held by FOpaqueStruct.
         * Since it is the default destructor, it performs standard cleanup without any
         * custom behavior.
         */
        ~FOpaqueStruct() = default;

        /**
         * Copy constructor for FOpaqueStruct.
         *
         * This constructor creates a new instance of FOpaqueStruct by copying the contents
         * from another FOpaqueStruct instance. It performs a deep copy to ensure the new
         * instance is an independent copy with its own storage.
         *
         * @param Other The FOpaqueStruct instance to copy from.
         * @return A new FOpaqueStruct instance that is a copy of the specified instance.
         */
        FOpaqueStruct(const FOpaqueStruct &Other);

        /**
         * Move constructor for FOpaqueStruct.
         *
         * This constructor initializes an FOpaqueStruct instance by transferring ownership
         * of the resources from another FOpaqueStruct instance. The original instance is
         * left in a valid but unspecified state.
         *
         * @param Other The FOpaqueStruct instance from which to move resources.
         * @return A new FOpaqueStruct instance that has taken ownership of the resources from Other.
         */
        FOpaqueStruct(FOpaqueStruct &&Other) noexcept = default;

        /**
         * Assignment operator for FOpaqueStruct.
         *
         * This operator overload allows one FOpaqueStruct instance to be assigned the contents
         * of another FOpaqueStruct instance. It performs a deep copy to ensure that the assigned
         * instance is a separate and independent copy with its own storage.
         *
         * @param Other The FOpaqueStruct instance to copy from.
         * @return A reference to the FOpaqueStruct instance with the copied data.
         */
        FOpaqueStruct &operator=(const FOpaqueStruct &Other);

        /**
         * Move assignment operator for FOpaqueStruct.
         *
         * This operator allows one FOpaqueStruct instance to take ownership
         * of the resources from another FOpaqueStruct instance.
         * The original instance is left in a valid but unspecified state after the assignment.
         *
         * @param Other The FOpaqueStruct instance from which to move resources.
         * @return A reference to the FOpaqueStruct instance that has taken ownership of the resources.
         */
        FOpaqueStruct &operator=(FOpaqueStruct &&Other) noexcept = default;

        /**
         * Retrieves the value stored within the FOpaqueStruct instance, ensuring it is of the specified type.
         *
         * This method verifies that the internal structure type matches the expected type T and then casts
         * the stored data to the required type. If the type check fails, it triggers an assertion.
         *
         * @return A reference to the value of type T stored within the FOpaqueStruct instance.
         */
        template <typename T>
            requires UEStruct<T>
        T &GetValue() const {
            check(GetStruct() == GetScriptStruct<T>())
            return *static_cast<T *>(Storage.get());
        }

        /**
         * @brief Retrieves the stored value from the opaque data structure.
         *
         * This function extracts the value contained within the FOpaqueStruct
         * instance, ensuring the structure's validity before accessing its storage.
         *
         * @return The stored value within the FOpaqueStruct instance.
         */
        void *GetValue() const;

        /**
         * @brief Attempts to retrieve a reference to a value of type T.
         *
         * This method checks if the internal structure matches the script structure
         * of the specified type T. If the structures match, it returns nullptr.
         * Otherwise, it returns a reference to the value of type T.
         *
         * @return Optional containing a reference to the value of type T or nullptr if the structure matches.
         */
        template <typename T>
            requires UEStruct<T>
        TOptional<T &> TryGet() const {
            if (GetStruct() == GetScriptStruct<T>()) {
                return nullptr;
            }

            return &GetValue<T>();
        }

        /**
         * Attempts to retrieve the value of the opaque data structure if it is valid.
         *
         * @return An optional containing the value if valid, otherwise nullptr.
         */
        TOptional<void> TryGet() const;

        /**
         * @brief Checks if the current instance holds a valid value.
         *
         * This method evaluates whether the storage is valid and the internal structure
         * is not a null pointer.
         * @return true if the storage is valid and the internal structure is non-null; otherwise, false.
         */
        FORCEINLINE bool HasValue() const {
            return Storage != nullptr && GetStruct() != nullptr;
        }

        /**
         * @brief Retrieves the UScriptStruct associated with the current storage.
         *
         * This method returns a pointer to the UScriptStruct that is linked with the
         * current instance of Storage. The structure contains metadata and layout
         * information about a specific data type used in Unreal Engine.
         *
         * @return A constant pointer to the UScriptStruct.
         */
        FORCEINLINE const UScriptStruct *GetStruct() const {
            return Storage.get_deleter().Struct;
        }

        /**
         * @brief Swaps the contents of this FOpaqueStruct with another FOpaqueStruct.
         *
         * This method exchanges the data contained in this FOpaqueStruct with that of
         * another FOpaqueStruct instance. The swap operation is performed in constant
         * time and does not involve copying the actual data.
         *
         * @param Other The FOpaqueStruct instance with which to swap contents.
         */
        void Swap(FOpaqueStruct &Other);

      private:
        static FStorage MakeStorage(const UScriptStruct *Struct) noexcept;
        FStorage PerformCopy() const;

        template <typename T, typename RawType = std::decay_t<T>>
            requires UEStruct<T>
        static FStorage InitializeFromKnown(T &&Struct) noexcept {
            auto ForwardStruct = []<typename U>(U &&S) {
                static_assert(std::same_as<U, T>);
                auto Memory = FMemory::Malloc(sizeof(RawType));
                *static_cast<RawType *>(Memory) = std::forward<U>(S);
                return Memory;
            };

            return FStorage(ForwardStruct(std::forward<T>(Struct)), FStructDeleter(GetScriptStruct<T>()));
        }

        template <typename T>
        static FStorage CopyFromUnknown(const T *Data, const UScriptStruct *Struct) noexcept {
            check(IsValid(Struct))
            ;
            if (Data == nullptr) {
                return FStorage(static_cast<void *>(nullptr), FStructDeleter(Struct));
            }

            auto CopyStruct = [](const T *D, const UScriptStruct *S) {
                auto Memory = FMemory::Malloc(static_cast<size_t>(S->GetStructureSize()));
                S->CopyScriptStruct(Memory, D);
                return Memory;
            };
            return FStorage(CopyStruct(Data, Struct), FStructDeleter(Struct));
        }

        FStorage Storage;
    };
} // namespace UE::Ranges