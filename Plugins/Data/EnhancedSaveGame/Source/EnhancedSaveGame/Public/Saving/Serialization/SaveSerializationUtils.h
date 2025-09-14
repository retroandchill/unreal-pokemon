// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Templates/SubclassOf.h"

#include "SaveSerializationUtils.generated.h"

/**
 * @struct FObjectData
 * @brief FObjectData is a structure used to serialize and deserialize UObject instances for saving and loading.
 *
 * This structure holds the class type and serialized data of a UObject. It provides static methods for serializing
 * a UObject instance into an FObjectData and deserializing an FObjectData back into a UObject.
 */
USTRUCT(BlueprintType)
struct ENHANCEDSAVEGAME_API FObjectData
{
    GENERATED_BODY()

    /**
     * @property ObjectClass
     * @brief Class type of the UObject being serialized.
     *
     * Specifies the class of the UObject whose data is being serialized. This property is used during
     * both serialization and deserialization processes to ensure that the correct class type is
     * handled. It is visible anywhere, can be read from Blueprints, saved in games, and categorized under
     * "Serialization".
     */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "Serialization")
    TSubclassOf<UObject> ObjectClass;

    /**
     * @property Data
     * @brief Serialized data of the UObject instance.
     *
     * This array holds the binary data representing the serialized state of a UObject. It is used
     * during the saving and loading processes to maintain the state of the UObject across sessions.
     * It is visible anywhere, can be read from Blueprints, saved in games, and categorized under "Serialization".
     */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "Serialization")
    TArray<uint8> Data;

    /**
     * @brief Serializes a UObject instance into an FObjectData structure.
     *
     * This method takes a UObject, serializes its state, and returns an FObjectData structure containing
     * the class type and serialized data of the UObject. If the UObject is invalid, it returns an empty FObjectData.
     *
     * @param Object The UObject instance to be serialized.
     * @return An FObjectData structure containing the class type and serialized data of the UObject.
     */
    static FObjectData SerializeObject(UObject *Object);

    /**
     * @brief Deserializes the FObjectData into a UObject instance.
     *
     * This method constructs a UObject from the serialized data stored in the FObjectData structure. It ensures that
     * the data is valid and the object can be properly reconstructed. If the ObjectClass is invalid or the data is
     * empty, it returns a nullptr.
     *
     * @return A pointer to the deserialized UObject instance, or nullptr if deserialization fails.
     */
    UObject *DeserializeObject() const;

    void DeserializeObject(UObject *Object) const;
};

/**
 * @class USaveSerializationUtils
 * @brief Utility class for serializing and deserializing UObjects for saving and loading.
 *
 * This class provides static methods to serialize a UObject into a binary data structure
 * and to deserialize the binary data back into a UObject. It is designed to be used in
 * blueprint scripts and integrates with Unreal Engine's gameplay saving functionality.
 */
UCLASS()
class ENHANCEDSAVEGAME_API USaveSerializationUtils : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * @brief Serializes a UObject into an FObjectData structure.
     *
     * This method serializes the given UObject instance and returns an FObjectData structure
     * containing the class type and serialized data. If the UObject is invalid, it returns an
     * empty FObjectData.
     *
     * @param Object The UObject instance to be serialized.
     * @return An FObjectData structure containing the class type and serialized data of the UObject.
     */
    UFUNCTION(BlueprintCallable, Category = "Serialization|Saving")
    static FObjectData SerializeObject(UObject *Object);

    /**
     * @brief Deserializes the given FObjectData into a UObject instance.
     *
     * This method reconstructs a UObject from the serialized data stored in the provided FObjectData
     * structure. It ensures that the serialized data is valid and reconstructs the UObject accordingly.
     * If the deserialization cannot be performed due to invalid class type or data, it returns nullptr.
     *
     * @param SerializedData The FObjectData containing the class type and serialized data of the UObject.
     * @return A pointer to the deserialized UObject instance, or nullptr if deserialization fails.
     */
    UFUNCTION(BlueprintCallable, Category = "Serialization|Saving")
    static UObject *DeserializeObject(const FObjectData &SerializedData);
};
