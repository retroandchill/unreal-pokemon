// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "EntitySaver.h"

#include "DefaultEntitySaver.generated.h"

/**
 * @class UDefaultEntitySaver
 * @brief A default implementation of the UEntitySaver class that provides basic entity saving support.
 *
 * This implementation is designed to act as a fallback entity saver with the lowest priority. Its primary
 * purpose is to provide saving support for any entity type by default.
 *
 * @details
 * - The priority is set to the minimum possible integer value to ensure it has the lowest precedence among various
 * entity savers.
 * - It supports all entity subclasses unconditionally by returning true for the `NativeSupports` method.
 *
 * @note This class is protected and is designed to serve as a base or default implementation.
 */
UCLASS()
class RPGSAVING_API UDefaultEntitySaver : public UEntitySaver
{
    GENERATED_BODY()
  protected:
    /**
     * @brief Constructor for the UDefaultEntitySaver class.
     *
     * Initializes the default entity saver with the lowest possible priority. This ensures the saver
     * acts as a fallback implementation for entity saving.
     *
     * @return A new instance of the UDefaultEntitySaver with minimum priority set.
     */
    UDefaultEntitySaver()
    {
        SetPriority(std::numeric_limits<int32>::min());
    }

    /**
     * @brief Determines whether the specified entity type is supported for saving by this saver.
     *
     * This method indicates that the UDefaultEntitySaver supports saving for all subclasses
     * of URPGEntity unconditionally.
     *
     * @param Entity The class type of the URPGEntity to check for saving support.
     * @return Always returns true, signifying that all URPGEntity subclasses are supported.
     */
    bool NativeSupports(TSubclassOf<URPGEntity> Entity) const override
    {
        return true;
    }
};
