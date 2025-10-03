// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "RPGEntitySaveDataExtensions.generated.h"

struct FAdditionalComponentDataKey;
class URPGComponent;
struct FRPGComponentSaveDataHandle;
class URPGEntity;
struct FRPGEntitySaveDataHandle;

/**
 * A utility class that provides extension methods related to FRPGEntitySaveDataHandle objects.
 * This class is part of the RPGSAVING_API module and inherits from UBlueprintFunctionLibrary
 * to allow the use of its static methods within Blueprints.
 */
UCLASS(meta = (InternalType))
class RPGSAVING_API URPGEntitySaveDataExtensions : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * Retrieves the version number of the entity save data.
     *
     * This method fetches the version information stored in the provided
     * FRPGEntitySaveDataHandle. The version is useful for identifying the
     * format or changes in the save data structure.
     *
     * @param SaveData The save data handle from which to retrieve the version.
     * @return The version number of the save data as an integer.
     */
    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static int32 GetVersion(const FRPGEntitySaveDataHandle &SaveData);

    /**
     * Retrieves the class type of the RPG entity associated with the specified save data handle.
     * This method allows access to the entity's class that is stored within the save data.
     *
     * @param SaveData A reference to the save data handle containing entity information.
     * @return The class type of the RPG entity associated with the provided save data handle.
     */
    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static TSubclassOf<URPGEntity> GetEntityClass(const FRPGEntitySaveDataHandle &SaveData);

    /**
     * Retrieves the required component data from the specified save data handle for a given property name.
     *
     * This method searches the components within the save data handle for the one associated with the specified
     * property name. If the required component is found, it assigns the component data to the output parameter and
     * returns true.
     *
     * @param SaveData The save data handle that contains the entity's components.
     * @param PropertyName The name of the property to locate within the save data's components.
     * @param OutComponentData A reference to the component save data where the result will be stored if found.
     * @return True if the required component is found and assigned to the output parameter; false otherwise.
     */
    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static bool GetRequiredComponent(const FRPGEntitySaveDataHandle &SaveData, FName PropertyName,
                                     FRPGComponentSaveDataHandle &OutComponentData);

    /**
     * Retrieves the additional component associated with a specific key from the save data handle.
     *
     * This method searches the additional components stored within the provided save data handle
     * for the one corresponding to the specified key. If the component is found, it assigns the
     * component's data to the output parameter and returns true. Otherwise, it returns false.
     *
     * @param SaveData The handle containing the entity's save data, including additional components.
     * @param Key The unique key used to identify the additional component within the save data.
     * @param OutComponentData A reference to the variable where the resulting component data will be stored if found.
     * @return True if the additional component is found and its data is assigned to the output parameter; false
     * otherwise.
     */
    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static bool GetAdditionalComponent(const FRPGEntitySaveDataHandle &SaveData, const FAdditionalComponentDataKey &Key,
                                       FRPGComponentSaveDataHandle &OutComponentData);
};
