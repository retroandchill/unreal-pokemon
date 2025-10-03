// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "RPGEntitySaveData.h"
#include "SaverBase.h"
#include "UObject/Object.h"

#include "EntitySaver.generated.h"

class URPGEntity;

/**
 * @class UEntitySaver
 * @brief An abstract base class for saving and loading RPG entity data.
 *
 * This class provides the foundation for managing the serialization and deserialization
 * of RPG entity data. It includes a set of overridable methods and Blueprint-implementable
 * events to support complex saving and loading operations.
 */
UCLASS(Abstract)
class RPGSAVING_API UEntitySaver : public USaverBase
{
    GENERATED_BODY()

  public:
    /**
     * @brief Checks if the specified RPG entity class is supported by the saver.
     *
     * This method determines whether a given RPG entity class can be handled by
     * this saver. It combines override functionalities with native support checks
     * to determine compatibility.
     *
     * @param Entity The class of the RPG entity to check for support.
     * @return true if the specified entity class is supported, false otherwise.
     */
    bool Supports(TSubclassOf<URPGEntity> Entity) const;

    /**
     * @brief Saves the data of the given RPG entity and returns the associated save data handle.
     *
     * This method manages the creation and population of an RPG entity save data handle based on
     * the specified entity. It combines native saving functionality with Blueprint-implementable
     * saving logic to ensure a comprehensive serialization process.
     *
     * @param Entity A pointer to the RPG entity to be saved. This entity must be compatible with
     *        the saver and provide the necessary data for serialization.
     * @return A handle to the created save data, representing the serialized state of the specified entity.
     */
    FRPGEntitySaveDataHandle SaveData(const URPGEntity *Entity) const;

    /**
     * @brief Loads entity data from the specified save data handle.
     *
     * This function is responsible for deserializing data into the given RPG entity
     * from the provided save data handle. The method combines native functionality
     * along with extendable support to allow Blueprint implementations to add custom logic.
     *
     * @param Entity A pointer to the RPG entity where the data will be loaded. This entity
     *        must be valid and compatible with the provided save data.
     * @param SaveData A reference to the save data handle representing the serialized
     *        entity state to be loaded.
     */
    void LoadData(URPGEntity *Entity, const FRPGEntitySaveDataHandle &SaveData) const;

  protected:
    /**
     * @brief Determines native support for a specified RPG entity class.
     *
     * This method checks whether the given RPG entity class is natively supported by the saver.
     * This is intended to be overridden by derived classes for defining specific compatibility logic.
     *
     * @param Entity The class of the RPG entity to check for native support.
     * @return true if this saver natively supports the specified entity class, false otherwise.
     */
    virtual bool NativeSupports(TSubclassOf<URPGEntity> Entity) const
    {
        return false;
    }

    /**
     * @brief Blueprint-implementable event to check if a specified RPG entity class is supported.
     *
     * This event provides the ability to extend the native entity support check functionality in Blueprints.
     * It allows developers to define additional criteria for determining whether a given RPG entity
     * class can be handled by this saver, apart from the native checks.
     *
     * @param Entity The class of the RPG entity to check for support.
     * @return true if the specified entity class is supported, false otherwise.
     */
    UFUNCTION(BlueprintImplementableEvent, DisplayName = "Supports", Category = "Saving",
              meta = (ScriptName = "Supports"))
    bool K2_Supports(TSubclassOf<URPGEntity> Entity) const;

    /**
     * @brief Saves the data of an RPG entity into the provided save data handle.
     *
     * This method serializes the components of the specified RPG entity and stores them
     * in the save data handle for persistence. It includes both required and additional
     * components of the entity.
     *
     * @param Entity The RPG entity whose data is being saved. This should include all its components.
     * @param SaveData The handle where the serialized entity data, including its components, will be stored.
     */
    virtual void NativeSaveData(const URPGEntity *Entity, const FRPGEntitySaveDataHandle &SaveData) const;

    /**
     * @brief Blueprint-implementable event to save the data of the specified RPG entity.
     *
     * This event provides a mechanism to serialize the given RPG entity's data into the referenced
     * save data handle. It allows developers to define the saving logic specifically within Blueprints.
     *
     * @param Entity A pointer to the RPG entity whose data is being saved. This entity must be valid
     *        and provide the necessary information for serialization.
     * @param SaveData A reference to the save data handle where the serialized state of the entity
     *        will be stored.
     */
    UFUNCTION(BlueprintImplementableEvent, DisplayName = "Save Data", Category = "Saving",
              meta = (ScriptName = "SaveData"))
    void K2_SaveData(const URPGEntity *Entity, const FRPGEntitySaveDataHandle &SaveData) const;

    /**
     * @brief Loads RPG entity data from the provided save handle.
     *
     * This method deserializes the entity's state, including required and additional components,
     * using the provided save data. It performs validation to ensure compatibility between
     * the entity's class and the save data and logs warnings for any discrepancies or failures
     * during the loading process.
     *
     * @param Entity Pointer to the RPG entity that requires its data to be loaded.
     * @param SaveData Reference to the save data handle that contains the serialized
     * entity state to be loaded.
     */
    virtual void NativeLoadData(URPGEntity *Entity, const FRPGEntitySaveDataHandle &SaveData) const;

    /**
     * @brief Implements the logic for loading RPG entity data for Blueprint interaction.
     *
     * This method is designed to be overridden in Blueprints to provide custom loading logic
     * for RPG entities. It enables deserialization of saved data into a target entity's
     * state and properties.
     *
     * @param Entity The RPG entity instance that will have its data loaded.
     * @param SaveData A handle containing the serialized save data for the entity.
     */
    UFUNCTION(BlueprintImplementableEvent, DisplayName = "Load Data", Category = "Saving",
              meta = (ScriptName = "LoadData"))
    void K2_LoadData(URPGEntity *Entity, const FRPGEntitySaveDataHandle &SaveData) const;
};
