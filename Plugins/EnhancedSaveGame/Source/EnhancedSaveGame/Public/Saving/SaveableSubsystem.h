// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Serialization/SaveSerializationUtils.h"
#include "UObject/Interface.h"
#include "SaveableSubsystem.generated.h"

class UEnhancedSaveGame;
class ISerializable;

// This class does not need to be modified.
UINTERFACE()
class USaveableSubsystem : public UInterface {
    GENERATED_BODY()
};

/**
 * A type of subsystem that can send data for saving.
 */
class ENHANCEDSAVEGAME_API ISaveableSubsystem {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    /**
     * Creates the save data for a given UEnhancedSaveGame instance.
     *
     * This method is intended to be overridden to define how the data
     * for an object should be saved into the UEnhancedSaveGame object.
     *
     * @param SaveGame The UEnhancedSaveGame instance where the data will be saved.
     */
    UFUNCTION(BlueprintNativeEvent, Category = Saving)
    void CreateSaveData(UEnhancedSaveGame* SaveGame) const;

    /**
     * Loads saved data into an object.
     *
     * This method is intended to be overridden to define how the data
     * for an object should be loaded from the provided FObjectData structure.
     *
     * @param ObjectData The FObjectData structure containing the serialized data to be loaded into the object.
     */
    UFUNCTION(BlueprintNativeEvent, Category = Saving)
    void LoadSaveData(const FObjectData &ObjectData);

};