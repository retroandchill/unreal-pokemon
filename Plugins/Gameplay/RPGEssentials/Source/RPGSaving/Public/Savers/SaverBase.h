// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "SaverBase.generated.h"

/**
 * @brief Abstract base class for saving-related functionality in the RPG Saving system.
 *
 * This class provides the foundational properties and methods for objects responsible for
 * saving data, including functionality for tracking versioning and priority. All savers
 * within the system inherit from this class.
 */
UCLASS(Abstract)
class RPGSAVING_API USaverBase : public UObject
{
    GENERATED_BODY()

  public:
    /**
     * @brief Retrieves the version number associated with this saver instance.
     *
     * This method provides access to the current version of the saver,
     * which may be used for version control and compatibility within the save system.
     *
     * @return The version number as an integer.
     */
    int32 GetVersion() const
    {
        return Version;
    }

    /**
     * @brief Retrieves the priority level of this saver instance.
     *
     * This method returns the priority value, which can be used to determine
     * the order in which savers are processed within the saving system.
     *
     * @return The priority level as an integer.
     */
    int32 GetPriority() const
    {
        return Priority;
    }

  protected:
    /**
     * @brief Sets the version number for this saver instance.
     *
     * This method updates the version number, which is used for versioning and
     * compatibility within the save system.
     *
     * @param NewVersion The new version number to be assigned to the saver.
     */
    void SetVersion(const int32 NewVersion)
    {
        Version = NewVersion;
    }

    /**
     * @brief Sets the new priority level for this saver instance.
     *
     * This method assigns a new priority value to the saver. The priority
     * level determines the processing order of savers within the saving system,
     * where higher priority savers are processed first.
     *
     * @param NewPriority The new priority value to be assigned.
     */
    void SetPriority(const int32 NewPriority)
    {
        Priority = NewPriority;
    }

  private:
    /**
     * @brief Represents the version number utilized for save data versioning.
     *
     * This property is used for maintaining compatibility between different
     * versions of the saving system. It allows tracking and control over
     * version-specific behaviors during save and load operations.
     */
    UPROPERTY(EditDefaultsOnly, Getter, Category = "Saving")
    int32 Version = 1;

    /**
     * @brief Specifies the priority level of the saver within the saving system.
     *
     * This property determines the order in which the saver is processed. Higher priority
     * values indicate that the saver will be processed earlier compared to those with
     * lower priority. This is used to control the sequencing of save operations.
     */
    UPROPERTY(EditDefaultsOnly, Getter, Category = "Saving")
    int32 Priority = 0;
};
