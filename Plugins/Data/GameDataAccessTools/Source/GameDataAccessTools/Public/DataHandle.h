// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

#include "DataHandle.generated.h"

/**
 * @class FDataHandleEntry
 *
 * @brief Represents an entry for managing and handling data in a structured format.
 *
 * The FDataHandleEntry class models a single data entry in a data management
 * system and provides the necessary methods and attributes for efficient
 * data handling. This class is commonly used as part of a larger system
 * for organizing, storing, or querying data.
 *
 * It typically includes functionality for tracking and accessing properties
 * related to the data entry, such as identifiers, metadata, or state.
 *
 * This class is suitable for use in systems that require structured
 * and dynamic handling of data components within their architecture.
 */
USTRUCT(meta = (RecordStruct, UseProperties, ReadOnly))
struct FDataHandleEntry
{
    GENERATED_BODY()

    /**
     * @class ID
     *
     * @brief Represents a unique identifier used for distinguishing entities or objects.
     *
     * The ID class encapsulates the concept of an identifier, commonly used to
     * uniquely reference or manage objects within a system. It serves as a fundamental
     * component in systems requiring identification and tracking of entities.
     *
     * This class can be employed in various domains such as database management,
     * authentication systems, or object lifecycle tracking, where unique identification
     * is critical to operations.
     */
    UPROPERTY(meta = (Required))
    FName ID;

    /**
     * @property DisplayName
     *
     * @brief Represents the display name of a data entry in a structured format.
     *
     * The DisplayName variable is of type FText and is used to store a human-readable
     * name for a data entry. This name is typically intended for display purposes
     * in user interfaces or other contexts where a textual representation of the
     * entry is required.
     *
     * This property is marked as required and is expected to have a value
     * associated with every instance of the data entry it pertains to. It is commonly
     * used in systems that categorize or present data entities to the user.
     */
    UPROPERTY(meta = (Required))
    FText DisplayName;
};
