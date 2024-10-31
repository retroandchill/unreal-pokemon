// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Pointers/PropertyDeleter.h"
#include "UObject/Object.h"

#include "PropertyEntry.generated.h"

/**
 * @class FPropertyEntry
 * @brief The FPropertyEntry class represents an entry for a property with details such as name, key, type, type name,
 * object image, and raw value.
 *
 * This class is intended for storing and managing detailed information about a property entry, including its name,
 * unique key, data type, type name, associated image object, and the property's raw value.
 *
 * @note Ensure proper handling of the raw value for different types to avoid data corruption or unexpected behavior.
 */
USTRUCT()
struct ENHANCEDSAVEGAME_API FPropertyEntry {
    GENERATED_BODY()

    /**
     * @brief A unique pointer to a property managed pointer.
     *
     * This variable holds a unique pointer to data managed by a custom deleter specifically for property objects.
     * It ensures proper memory management, including allocation and deallocation of the property data.
     *
     * @note Use this variable to safely and conveniently manage property data lifetimes.
     */
    UE::Ranges::FUniquePropertyPtr Data;

    /**
     * @variable OwningObject
     * @brief A pointer to the owning UObject for the property.
     *
     * This variable stores a TObjectPtr to the UObject that owns the property.
     * It ensures that the property's owner can be tracked and retained by the GC to avoid the property from being
     * deleted out from under us.
     */
    UPROPERTY()
    TObjectPtr<UObject> OwningObject;

    /**
     * @brief Default constructor for the FPropertyEntry class.
     *
     * Initializes a new instance of the FPropertyEntry class with default values.
     *
     * @return A new instance of FPropertyEntry with default initialization.
     */
    FPropertyEntry() = default;

    /**
     * @brief Constructs an FPropertyEntry object with a given property object.
     *
     * This constructor initializes the FPropertyEntry with a unique pointer to the provided property data,
     * and sets the owning UObject.
     *
     * @param Property A pointer to the FProperty object that defines the property's characteristics and its owning
     * UObject.
     * @return An instance of FPropertyEntry initialized with the provided property.
     */
    explicit FPropertyEntry(const FProperty *Property);

    /**
     * @brief Constructs an FPropertyEntry object with the specified data and property.
     *
     * This constructor initializes the FPropertyEntry with a unique pointer to the provided data, managed by
     * the FProperty object, and sets the owning UObject.
     *
     * @param Data A pointer to the data of type T to be managed within this FPropertyEntry.
     * @param Property A pointer to the FProperty object that defines the property's characteristics and its owning
     * UObject.
     */
    template <typename T>
    FPropertyEntry(T *Data, const FProperty *Property)
        : Data(UE::Ranges::MakeUniqueProperty<T>(Data, Property)), OwningObject(Property->GetOwnerUObject()) {
    }

    /**
     * @brief Copy constructor for the FPropertyEntry class.
     *
     * This constructor creates a new instance of FPropertyEntry by copying the
     * data from an existing FPropertyEntry object. It duplicates the data managed
     * by the unique pointer and sets the owning UObject to match the provided
     * FPropertyEntry object.
     *
     * @param Other A const reference to the FPropertyEntry object to be copied.
     * @return A new instance of FPropertyEntry initialized with the copied data.
     */
    FPropertyEntry(const FPropertyEntry &Other);

    /**
     * @brief Move constructor for the FPropertyEntry class.
     *
     * This constructor initializes a new instance of FPropertyEntry by transferring ownership of the data
     * from an existing FPropertyEntry object to the new instance.
     *
     * @param Other An r-value reference to the FPropertyEntry object to be moved.
     * @return A new instance of FPropertyEntry with ownership of the data from the provided object.
     */
    FPropertyEntry(FPropertyEntry &&Other) = default;

    /**
     * @brief Destructor for the FPropertyEntry class.
     *
     * Cleans up any associated resources for an instance of the FPropertyEntry class.
     *
     * As this destructor is defined as default, it relies on the compiler-generated
     * destructor to handle the cleanup, ensuring proper destruction of member variables,
     * including managed pointers and owned objects.
     */
    ~FPropertyEntry() = default;

    /**
     * @brief Copy assignment for the FPropertyEntry class.
     *
     * This assignment modifies an existing instance of FPropertyEntry by copying the
     * data from an existing FPropertyEntry object. It duplicates the data managed
     * by the unique pointer and sets the owning UObject to match the provided
     * FPropertyEntry object.
     *
     * @param Other A const reference to the FPropertyEntry object to be copied.
     * @return The modified FPropertyEntry.
     */
    FPropertyEntry &operator=(const FPropertyEntry &Other);

    /**
     * @brief Move assignment for the FPropertyEntry class.
     *
     * This assignment modifies an existing instance of FPropertyEntry by transferring ownership of the data
     * from an existing FPropertyEntry object to the new instance.
     *
     * @param Other An r-value reference to the FPropertyEntry object to be moved.
     * @return The modified FPropertyEntry.
     */
    FPropertyEntry &operator=(FPropertyEntry &&Other) = default;

    /**
     * Serializes the data for saving and loading.
     *
     * @param Ar The archive to use when saving the data.
     * @return Did serialization succeed.
     */
    bool Serialize(FArchive &Ar);
};

/**
 * @struct TStructOpsTypeTraits
 * @brief Specializes the struct operations type traits for FPropertyEntry.
 *
 * This specialization configures various serialization behaviors for the FPropertyEntry type, indicating how the type
 * should be handled during serialization and network-related operations.
 *
 * The following serialization traits are specified:
 * - WithSerializer: Indicates whether the type supports serialization.
 * - WithNetSerializer: Indicates whether the type supports network serialization.
 * - WithNetSharedSerialization: Indicates whether the type supports shared network serialization.
 */
template <>
struct TStructOpsTypeTraits<FPropertyEntry> : TStructOpsTypeTraitsBase2<FPropertyEntry> {
    enum {
        WithSerializer = true,
        WithNetSerializer = false,
        WithNetSharedSerialization = false,
    };
};