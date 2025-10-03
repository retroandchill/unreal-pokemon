// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "RPGEntitySaveData.h"
#include "SaverBase.h"
#include "UObject/Object.h"

#include "ComponentSaver.generated.h"

/**
 * @class UComponentSaver
 * @brief Abstract base class responsible for saving and loading RPG component data.
 *
 * This class provides functionality for determining whether a specific RPG component
 * is supported, saving its data, and loading data back into the component. It serves
 * as a foundational class for creating custom save systems for RPG components.
 */
UCLASS(Abstract)
class RPGSAVING_API UComponentSaver : public USaverBase
{
    GENERATED_BODY()

  public:
    /**
     * @brief Determines if the specified RPG component is supported by this component saver.
     *
     * This method checks both the blueprint-implemented and native implementations
     * to determine if the given RPG component class is supported for saving and loading.
     *
     * @param Component The class type of the RPG component to check for support.
     * @return True if the specified RPG component is supported, otherwise false.
     */
    bool Supports(TSubclassOf<URPGComponent> Component) const;

    /**
     * @brief Saves the state of the specified RPG component into a save data handle.
     *
     * This method creates a new instance of FRPGComponentSaveDataHandle to store the data
     * of the provided RPG component. It combines both native and blueprint-implemented logic
     * to populate the save data. The resulting handle contains the component's class information
     * and current data version for proper serialization and deserialization.
     *
     * @param Component Pointer to the URPGComponent whose state needs to be saved.
     * @return The populated FRPGComponentSaveDataHandle containing the saved data.
     */
    FRPGComponentSaveDataHandle SaveData(const URPGComponent *Component) const;

    /**
     * @brief Loads data into the specified RPG component from a provided save data handle.
     *
     * This method applies saved data to a given RPG component by utilizing both native
     * and blueprint-implemented logic. The function ensures that the component's state
     * is restored based on the information contained within the save data handle.
     *
     * @param Component Pointer to the URPGComponent into which data will be loaded.
     * @param SaveData A reference to the FRPGComponentSaveDataHandle containing data to be applied to the component.
     */
    void LoadData(URPGComponent *Component, const FRPGComponentSaveDataHandle &SaveData) const;

  protected:
    /**
     * @brief Determines if the specified RPG component is natively supported for saving and loading.
     *
     * This method provides the native implementation to verify if a given RPG component
     * class can be handled by this component saver. It can be overridden by subclasses
     * to add specific support logic.
     *
     * @param Component The class type of the RPG component to check for native support.
     * @return True if the specified RPG component is natively supported, otherwise false.
     */
    virtual bool NativeSupports(TSubclassOf<URPGComponent> Component) const
    {
        return false;
    }

    /**
     * @brief Blueprint-implementable method to check if the specified RPG component is supported.
     *
     * This method allows blueprint classes to provide their own implementation for determining
     * if the given RPG component class is supported for saving and loading. It complements the native
     * implementation of the Supports method.
     *
     * @param Component The class type of the RPG component to check for support.
     * @return True if the specified RPG component is supported by the blueprint implementation, otherwise false.
     */
    UFUNCTION(BlueprintImplementableEvent, DisplayName = "Supports", Category = "Saving",
              meta = (ScriptName = "Supports"))
    bool K2_Supports(TSubclassOf<URPGComponent> Component) const;

    /**
     * @brief Saves the data of the specified RPG component into the provided save data handle.
     *
     * This method handles the native serialization logic for saving the state of the specified
     * RPG component. It is intended to be overridden in subclasses to implement specific
     * save logic for different types of RPG components.
     *
     * @param Component Pointer to the URPGComponent whose data needs to be saved.
     * @param SaveData Reference to a FRPGComponentSaveDataHandle that will store the saved data.
     */
    virtual void NativeSaveData(const URPGComponent *Component, const FRPGComponentSaveDataHandle &SaveData) const;

    /**
     * @brief Blueprint-implementable method to save the state of an RPG component into a save data handle.
     *
     * This method provides a blueprint hook for saving the state of the given RPG component
     * into the provided save data handle. It allows custom implementations to be defined
     * in blueprints for handling the save process of specific RPG components.
     *
     * @param Component Pointer to the URPGComponent whose state needs to be saved.
     * @param SaveData A reference to the FRPGComponentSaveDataHandle that will store the saved data.
     */
    UFUNCTION(BlueprintImplementableEvent, DisplayName = "Save Data", Category = "Saving",
              meta = (ScriptName = "SaveData"))
    void K2_SaveData(const URPGComponent *Component, const FRPGComponentSaveDataHandle &SaveData) const;

    /**
     * @brief Loads data from the given save handle into the specified RPG component.
     *
     * This method allows the deserialization of saved data into the provided RPG component.
     * It is intended to be used in save systems to restore component states.
     *
     * @param Component Pointer to the RPG component that the data will be loaded into.
     * @param SaveData Handle containing the serialized data for the RPG component.
     */
    virtual void NativeLoadData(URPGComponent *Component, const FRPGComponentSaveDataHandle &SaveData) const;

    /**
     * @brief Blueprint-implementable event responsible for loading data into an RPG component.
     *
     * This function is intended to be implemented in Blueprint to handle the loading of saved data
     * into the specified RPG component using the provided save data handle.
     *
     * @param Component The RPG component into which the saved data will be loaded.
     * @param SaveData The handle containing the saved data to be loaded into the component.
     */
    UFUNCTION(BlueprintImplementableEvent, DisplayName = "Load Data", Category = "Saving",
              meta = (ScriptName = "LoadData"))
    void K2_LoadData(URPGComponent *Component, const FRPGComponentSaveDataHandle &SaveData) const;
};
