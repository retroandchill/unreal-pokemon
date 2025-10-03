// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGEntitySaveData.h"

#include "RPGComponentSaveDataHandleExtensions.generated.h"

class URPGComponent;
struct FRPGComponentSaveDataHandle;

/**
 * @class URPGComponentSaveDataHandleExtensions
 * @brief A utility class providing blueprint-accessible functionality related to FRPGComponentSaveDataHandle.
 *
 * This class extends functionality for creating, accessing, and manipulating data associated
 * with RPG component save data handles. The methods are designed to simplify integration with
 * the Unreal Engine blueprint system.
 */
UCLASS(meta = (InternalType))
class RPGSAVING_API URPGComponentSaveDataHandleExtensions : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * @brief Creates a new FRPGComponentSaveDataHandle based on the specified component class and version.
     *
     * This method constructs and returns a handle for saving or loading data related to a specific RPG component
     * subclass. Useful for initializing new save data handles tailored to a defined component and version.
     *
     * @param ComponentClass The subclass of URPGComponent associated with the save data handle.
     * @param Version The version identifier for the save data, used to track changes in data structure or format.
     * @return A new instance of FRPGComponentSaveDataHandle initialized with the given component class and version.
     */
    UFUNCTION(meta = (ScriptMethod))
    static FRPGComponentSaveDataHandle CreateNew(TSubclassOf<URPGComponent> ComponentClass, int32 Version = 1);

    /**
     * @brief Retrieves the version number of the specified RPG component save data handle.
     *
     * This method provides the version number associated with the given save data handle,
     * which is used to track changes and ensure compatibility with the save data format.
     *
     * @param SaveData A reference to the FRPGComponentSaveDataHandle whose version is to be retrieved.
     * @return The version number of the save data stored in the provided handle.
     */
    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static int32 GetVersion(const FRPGComponentSaveDataHandle &SaveData);

    /**
     * @brief Retrieves the class type of the component associated with the given save data handle.
     *
     * This method extracts and returns the component class type from the provided
     * FRPGComponentSaveDataHandle, enabling further operations or reference to this component.
     *
     * @param SaveData The save data handle containing information about a specific RPG component.
     * @return The class type of the component associated with the save data handle.
     */
    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static TSubclassOf<URPGComponent> GetComponentClass(const FRPGComponentSaveDataHandle &SaveData);

    /**
     * @brief Retrieves data associated with a specific FRPGComponentSaveDataHandle.
     *
     * This method is intended to retrieve specific integer-based data from the provided save data handle.
     * It is used internally in relation to RPG component save data. The method is not expected to be directly called by
     * users.
     *
     * @param SaveData A reference to the FRPGComponentSaveDataHandle from which to retrieve the data.
     * @param OutData An out parameter where the retrieved integer data will be stored.
     * @return A boolean value indicating whether the data retrieval was successful.
     */
    UFUNCTION(CustomThunk, meta = (ScriptMethod, CustomStructureParam = OutData, ExtensionMethod))
    static bool GetData(const FRPGComponentSaveDataHandle &SaveData, int32 &OutData);
    DECLARE_FUNCTION(execGetData);

    /**
     * @brief Sets data associated with a specific FRPGComponentSaveDataHandle.
     *
     * This method is designed for internal use and is not intended to be directly called by users. It allows
     * assigning integer-based data to a given save data handle.
     *
     * @param SaveData A reference to the FRPGComponentSaveDataHandle to which data will be assigned.
     * @param Data The integer data value to set within the specified save data handle.
     */
    UFUNCTION(CustomThunk, meta = (ScriptMethod, CustomStructureParam = Data, ExtensionMethod))
    static void SetData(const FRPGComponentSaveDataHandle &SaveData, const int32 &Data);
    DECLARE_FUNCTION(execSetData);

    /**
     * @brief Retrieves the UScriptStruct associated with the provided FRPGComponentSaveDataHandle.
     *
     * This method returns the underlying UScriptStruct representing the data structure
     * for a given RPG component's save data handle.
     *
     * @param SaveData A reference to the FRPGComponentSaveDataHandle for which the UScriptStruct is to be retrieved.
     * @return A pointer to the UScriptStruct associated with the data in the specified save data handle.
     */
    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static const UScriptStruct *GetSaveDataStruct(const FRPGComponentSaveDataHandle &SaveData);
};
