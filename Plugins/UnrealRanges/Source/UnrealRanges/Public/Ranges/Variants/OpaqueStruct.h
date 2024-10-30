// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Structs.h"
#include <array>

namespace UE::Ranges {
    /**
     * @brief Represents an opaque structure.
     *
     * FOpaqueStruct is an abstract data type (ADT) used to encapsulate an Unreal defined structure. This works by
     * accessing Unreal Engine's reflection system to construct a structure of unknown type and size. This type utilizes
     * small buffer optimization allowing smaller structs to be stored in place.
     */
    class UNREALRANGES_API FOpaqueStruct {
    public:
        /**
         * The size of the buffer used for small structs.
         */
        static constexpr size_t SmallBufferSize = 32;

        /**
         * Check to determine if a given type fits inside the small buffer for the type.
         * @tparam T The type to check
         */
        template <typename T>
            requires UEStruct<T>
        static constexpr bool bFitsSmallBuffer = sizeof(std::decay_t<T>) <= SmallBufferSize;

        /**
         * @brief Default constructor for FOpaqueStruct.
         *
         * Initializes a FOpaqueStruct instance with no struct data.
         */
        FOpaqueStruct() = default;

        /**
         * @brief Constructs a FOpaqueStruct from a given UScriptStruct.
         *
         * This constructor initializes the opaque structure and determines the appropriate storage strategy based on
         * the structure size.
         *
         * @param Struct Pointer to the UScriptStruct used to initialize the FOpaqueStruct.
         * @return A new FOpaqueStruct instance encapsulating the provided UScriptStruct.
         */
        explicit FOpaqueStruct(const UScriptStruct* Struct) noexcept;

        /**
         * @brief Destructor for FOpaqueStruct.
         *
         * Cleans up and deallocates any resources held by the FOpaqueStruct instance. This includes releasing memory for any stored structure, regardless of whether it used the small buffer optimization or a larger allocation.
         */
        ~FOpaqueStruct();

        /**
         * @brief Copy constructor for FOpaqueStruct.
         *
         * This constructor initializes a new FOpaqueStruct instance by copying the contents of another FOpaqueStruct instance.
         *
         * @param Other The FOpaqueStruct instance to be copied.
         * @return A new FOpaqueStruct instance that is a copy of the provided instance.
         */
        FOpaqueStruct(const FOpaqueStruct& Other);

        /**
         * @brief Move constructor for FOpaqueStruct.
         *
         * This constructor initializes a new FOpaqueStruct instance by taking ownership of the resources from another FOpaqueStruct instance.
         *
         * @param Other The FOpaqueStruct instance to be moved.
         * @return A new FOpaqueStruct instance that has taken ownership of the provided instance's resources.
         */
        FOpaqueStruct(FOpaqueStruct&& Other) noexcept;

        /**
         * @brief Overloaded copy assignment operator for FOpaqueStruct.
         *
         * This operator overwrites the contents of this FOpaqueStruct instance by copying the contents of another FOpaqueStruct instance.
         *
         * @param Other The other struct to pull from
         * @return A reference to the first struct
         */
        FOpaqueStruct& operator=(const FOpaqueStruct& Other);

        /**
         * @brief Overloaded move assignment operator for FOpaqueStruct.
         *
         * This operator overwrites the contents of this FOpaqueStruct instance by taking ownership of the resources from another FOpaqueStruct instance.
         *
         * @param Other The other struct to pull from
         * @return A reference to the first struct
         */
        FOpaqueStruct& operator=(FOpaqueStruct&& Other) noexcept;

        /**
         * @brief Retrieves the value of the stored structure.
         *
         * This method checks if the stored structure matches the expected type and then returns a reference to the value.
         * If the structure fits within the small buffer, it retrieves the value from the small storage buffer.
         * Otherwise, it retrieves the value from the larger allocated storage.
         *
         * @tparam T The type of the structure expected.
         * @return A reference to the value of the stored structure.
         */
        template <typename T>
            requires UEStruct<T>
        T& GetValue() const {
            check(Struct == GetScriptStruct<T>())
            if constexpr (bFitsSmallBuffer<T>) {
                return *reinterpret_cast<T *>(Storage.SmallStorage.data());
            } else {
                return *static_cast<T*>(Storage.LargeStorage);
            }
        }

        /**
         * @brief Returns the value stored within the opaque structure.
         *
         * This method provides access to the value encapsulated in the FOpaqueStruct. It handles both
         * small and large structures by differentiating based on the structure's size.
         *
         * @return A pointer to the value stored in the FOpaqueStruct.
         */
        void* GetValue() const;

        
        /**
         * @brief Attempts to retrieve a reference to a stored value of type T.
         *
         * The method checks if the stored structure matches the type T. If it matches,
         * it returns a reference to the value. Otherwise, it returns a null optional.
         *
         * @return An optional reference to the stored value of type T. If the type
         *         does not match, a null optional is returned.
         */
        template <typename T>
            requires UEStruct<T>
        TOptional<T&> TryGet() const {
            if (Struct == GetScriptStruct<T>()) {
                return nullptr;
            }
            
            return &GetValue<T>();
        }

        /**
         * @brief Attempts to retrieve the value stored in the opaque structure.
         *
         * This method checks if the internal structure is valid and, if so, returns its value. If the structure is not valid,
         * it returns null.
         *
         * @return TOptional<void> containing the value if the structure is valid, otherwise null.
         */
        TOptional<void> TryGet() const;

        /**
         * @brief Checks if the opaque structure has an assigned value.
         *
         * This method verifies whether the encapsulated Unreal defined structure
         * within the FOpaqueStruct is valid by ensuring that the internal pointer
         * to the structure is not null.
         *
         * @return true if the structure is assigned and valid, false otherwise.
         */
        bool HasValue() const {
            return Struct != nullptr;
        }

        /**
         * @brief Retrieves the script structure associated with this instance.
         *
         * This method returns a pointer to the UStruct that represents the underlying structure.
         *
         * @return A constant pointer to the UStruct representing the script structure.
         */
        const UStruct* GetScriptStruct() const {
            return Struct;
        }

        /**
         * @brief Swaps the contents of this opaque structure with another.
         *
         * This method exchanges the contents of this FOpaqueStruct instance with the contents of the specified
         * FOpaqueStruct instance, effectively swapping the data between the two structures.
         *
         * @param Other A reference to another FOpaqueStruct instance whose contents will be swapped with this instance.
         */
        void Swap(FOpaqueStruct& Other);

    private:
        union FStorage {
            std::array<std::byte, SmallBufferSize> SmallStorage;
            void* LargeStorage;

            FStorage() = default;

            explicit FStorage(const UScriptStruct* Struct);
        };
        
        void DeleteExisting();
        FStorage PerformCopy() const;
        
        FStorage Storage;
        TObjectPtr<const UScriptStruct> Struct;
    };
}
