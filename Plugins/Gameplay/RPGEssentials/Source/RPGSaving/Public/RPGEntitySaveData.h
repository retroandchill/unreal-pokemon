// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "StructUtils/StructView.h"
#include "UObject/Object.h"

#include "RPGEntitySaveData.generated.h"

class URPGComponent;
class URPGEntity;

/**
 * FRPGComponentSaveData is a structure designed to encapsulate the save data for an RPGComponent.
 * It contains information required to rebuild or restore the state of a component in the game.
 */
USTRUCT(BlueprintType, meta = (UseProperties))
struct FRPGComponentSaveData final
{
    GENERATED_BODY()

    /**
     * Version represents the data version number for this component's save data.
     * It is utilized to track and manage compatibility or changes when saving and loading the component state.
     */
    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 Version = 1;

    /**
     * ComponentClass defines the class type of an RPGComponent that is associated with this save data.
     * It allows specifying the specific subclass of URPGComponent to be stored and later reconstructed during loading.
     * The variable is marked for blueprint accessibility and persistence through save games.
     */
    UPROPERTY(BlueprintReadWrite, SaveGame)
    TSubclassOf<URPGComponent> ComponentClass;

    /**
     * Data is a property designed to store instance-specific structured data for an RPG component.
     * It encapsulates detailed information required for reconstructing the internal state during gameplay or when
     * loading saved data. This property supports blueprint editing and is marked for persistence in save game
     * functionality.
     */
    UPROPERTY(BlueprintReadWrite, SaveGame)
    FInstancedStruct Data;

    /**
     * Default constructor for the FRPGComponentSaveData structure.
     * Initializes an instance of FRPGComponentSaveData with default values.
     * This constructor ensures that the object is created with its default state without any specific initialization
     * requirements.
     */
    FRPGComponentSaveData() = default;

    /**
     * Constructs an instance of FRPGComponentSaveData with specified component class and version.
     * This constructor allows initializing the save data structure with a particular class type
     * and a version number for tracking compatibility or changes.
     *
     * @param ComponentClass The class type of the RPGComponent associated with this save data.
     * @param Version The version number of the save data format, with a default value of 0.
     */
    explicit FRPGComponentSaveData(const TSubclassOf<URPGComponent> ComponentClass, const int32 Version = 0)
        : Version(Version), ComponentClass(ComponentClass)
    {
    }
};

/**
 * FRPGComponentSaveDataHandle is a handle designed to manage and encapsulate the save data
 * associated with an RPGComponent. It serves as an abstraction over the save data,
 * providing methods to access and manipulate stored component data and facilitating
 * versioning and serialization.
 *
 * The structure primarily operates by wrapping an internally managed data object,
 * which contains specifics about a component's version and its configuration.
 *
 * It supports:
 * - Retrieving the version of the saved component data.
 * - Accessing the class type of the component associated with the save.
 * - Accessing the instanced save data tied to the component.
 * - Serializing the component save data for storage or replication.
 */
USTRUCT(BlueprintType)
struct FRPGComponentSaveDataHandle
{
    GENERATED_BODY()

    /**
     * Default constructor for FRPGComponentSaveDataHandle.
     * This initializes an empty handle for RPG component save data.
     *
     * @return A default-constructed FRPGComponentSaveDataHandle object with no data.
     */
    FRPGComponentSaveDataHandle() = default;

    /**
     * Constructs an FRPGComponentSaveDataHandle with a specified component class and version.
     * This constructor initializes the handle with save data representing the given component class
     * and its associated version.
     *
     * @param ComponentClass The subclass of URPGComponent to be associated with the save data handle.
     * @param Version The version number of the save data. Defaults to 0 if not specified.
     */
    explicit FRPGComponentSaveDataHandle(TSubclassOf<URPGComponent> ComponentClass, int32 Version = 0)
        : Data(MakeShared<FRPGComponentSaveData>(ComponentClass, Version))
    {
    }

    /**
     * Retrieves the version number of the associated save data.
     * The version number is used to track changes and compatibility for the save data format.
     *
     * @return The version number of the save data stored in the handle.
     */
    int32 GetVersion() const
    {
        return Data->Version;
    }

    /**
     * Retrieves the class type of the RPG component associated with the save data.
     * The returned class represents the type of component that the save data is linked to.
     *
     * @return The subclass of URPGComponent associated with the save data.
     */
    TSubclassOf<URPGComponent> GetComponentClass() const
    {
        return Data->ComponentClass;
    }

    /**
     * Retrieves the instanced struct data associated with the RPG component.
     * The returned data can be used to inspect or manipulate the state of the component
     * stored within the save data handle.
     *
     * @return A reference to the FInstancedStruct data object.
     */
    FInstancedStruct &GetData() const
    {
        return Data->Data;
    }

    /**
     * Serializes the RPG component save data using the specified archive slot.
     * This method ensures that the save data is appropriately serialized or deserialized
     * when saving or loading the state of the RPG component.
     *
     * @param Ar The archive slot used for the serialization or deserialization operation.
     * @return True to indicate that the serialization operation was successful.
     */
    bool Serialize(const FStructuredArchive::FSlot Ar)
    {
        static FRPGComponentSaveData Defaults;
        FRPGComponentSaveData::StaticStruct()->SerializeItem(Ar, &Data.Get(), &Defaults);
        return true;
    }

  private:
    TSharedRef<FRPGComponentSaveData> Data = MakeShared<FRPGComponentSaveData>();
};

/**
 * TStructOpsTypeTraits<FRPGComponentSaveDataHandle> is a specialization of the TStructOpsTypeTraitsBase2
 * template for the FRPGComponentSaveDataHandle structure.
 *
 * This traits structure defines specific operational characteristics for the FRPGComponentSaveDataHandle,
 * enabling or disabling various functionality based on the type's requirements.
 *
 * Key features included in this specialization are:
 * - Structured serialization support is enabled through the WithStructuredSerializer trait.
 * - Network serialization and shared network serialization are explicitly disabled through the
 *   WithNetSerializer and WithNetSharedSerialization traits, respectively.
 */
template <>
struct TStructOpsTypeTraits<FRPGComponentSaveDataHandle> : TStructOpsTypeTraitsBase2<FRPGComponentSaveDataHandle>
{
    enum
    {
        WithStructuredSerializer = true,
        WithNetSerializer = false,
        WithNetSharedSerialization = false
    };
};

/**
 * FAdditionalComponentDataKey is a structure used to act as a unique identifier
 * for additional component data within a system. It facilitates the association
 * and retrieval of supplementary data tied to specific components.
 */
USTRUCT(BlueprintType, meta = (Readonly, RecordStruct, UseProperties))
struct FAdditionalComponentDataKey
{
    GENERATED_BODY()

    /**
     * Represents the class type of an RPGComponent that can be assigned to an entity.
     * This variable is used to specify the component class type during runtime or saving/loading operations.
     * It supports Blueprint usage and can be serialized for save game functionality.
     */
    UPROPERTY(BlueprintReadWrite, SaveGame)
    TSubclassOf<URPGComponent> ComponentClass;

    /**
     * Index is an integer that serves as an identifier or positional reference
     * for an additional component associated with an RPG entity.
     * It is used in conjunction with ComponentClass to uniquely identify or manage
     * component data, particularly during save or load operations.
     * This variable supports Blueprint usage and can be serialized for save game functionality.
     */
    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 Index = 0;

    /**
     * Default constructor for the FAdditionalComponentDataKey structure.
     * Initializes an instance with no specific component class or index.
     * This constructor is used to create an uninitialized or default state
     * of the FAdditionalComponentDataKey.
     *
     * @return A default-initialized instance of FAdditionalComponentDataKey.
     */
    FAdditionalComponentDataKey() = default;

    /**
     * Constructs an FAdditionalComponentDataKey instance with the specified component class and optional index.
     * This constructor is used to uniquely define or identify additional components associated with an RPG entity,
     * particularly for save or load operations.
     *
     * @param ComponentClass The class type of the RPGComponent associated with this key.
     * @param Index Optional index to further differentiate components of the same class. Default value is 0.
     * @return An initialized instance of FAdditionalComponentDataKey with the provided class and index.
     */
    explicit FAdditionalComponentDataKey(const TSubclassOf<URPGComponent> ComponentClass, const int32 Index = 0)
        : ComponentClass(ComponentClass), Index(Index)
    {
    }

    /**
     * This is an overloaded operator designed to implement custom behavior for a specific operation.
     * It allows the definition of unique functionality when the operator is used with instances of the associated
     * class.
     *
     * @return The result of the operation defined by the overloaded operator.
     */
    bool operator==(const FAdditionalComponentDataKey &) const = default;

    /**
     * GetTypeHash is a function used to generate a hash value for a given object.
     * It provides a unique hash for the input, aiding in functions such as object lookups
     * in hash-based containers like hash tables or hash maps.
     *
     * @param Data The object for which the hash value needs to be generated.
     * @return A hash value uniquely representing the provided object.
     */
    FORCEINLINE friend uint32 GetTypeHash(const FAdditionalComponentDataKey &Data)
    {
        return HashCombine(GetTypeHash(Data.ComponentClass), GetTypeHash(Data.Index));
    }
};

/**
 * FRPGEntitySaveData is a structure that encapsulates the save data necessary for preserving
 * and restoring the state of an entity in an RPG-style game. It includes versioning, class
 * type information, and references to both required and additional components associated
 * with the entity.
 */
USTRUCT(BlueprintType, meta = (UseProperties))
struct FRPGEntitySaveData final
{
    GENERATED_BODY()

    /**
     * Version represents the serialized version number of this entity's save data.
     * It is used to manage compatibility across different versions of the saved game structure.
     */
    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 Version = 1;

    /**
     * EntityClass represents a blueprintable subclass of URPGEntity associated with the RPG entity save data.
     * It specifies the class type of the entity that can be dynamically instantiated or referenced during gameplay.
     * This property is marked for saving and is editable in both Blueprint and runtime contexts.
     */
    UPROPERTY(BlueprintReadWrite, SaveGame)
    TSubclassOf<URPGEntity> EntityClass;

    /**
     * RequiredComponents is a property that maps specific RPG component types to their corresponding save data handles.
     * Each key represents a field path to an object property defining a required component type, while the value
     * links to the associated save data handle for restoring the component's state during gameplay.
     *
     * This property is marked for Blueprint read/write access and is included in save game serialization.
     * It is utilized to maintain a list of components that are essential for the correct functionality of an entity.
     */
    UPROPERTY(BlueprintReadWrite, SaveGame)
    TMap<TFieldPath<FObjectProperty>, FRPGComponentSaveDataHandle> RequiredComponents;

    /**
     * AdditionalComponents is a mapping of additional RPGComponent data associated with an entity.
     * Each key in the map corresponds to a unique identifier for a component, while the value is a handle
     * to the save data of that component. This structure allows for dynamic extensibility by supporting
     * components that are not statically required but might be added during gameplay.
     *
     * This property is designed for Blueprint read/write access and is included in save game serialization.
     * It supports the persistent management of dynamically added components to ensure their states
     * are properly restored when loading a saved game.
     */
    UPROPERTY(BlueprintReadWrite, SaveGame)
    TMap<FAdditionalComponentDataKey, FRPGComponentSaveDataHandle> AdditionalComponents;

    /**
     * Default constructor for the FRPGEntitySaveData structure.
     * This initializes an instance of the save data structure with default values.
     *
     * @return A newly constructed instance of FRPGEntitySaveData with default initializations.
     */
    FRPGEntitySaveData() = default;

    /**
     * Constructs an instance of FRPGEntitySaveData with the specified entity class and version.
     * This constructor allows initialization of the entity save data with a specific class type
     * and an optional version number to ensure compatibility with serialization across versions.
     *
     * @param EntityClass Specifies the class of the RPG Entity to associate with this save data.
     *                     This is a blueprintable subclass of URPGEntity.
     * @param Version Optional parameter that specifies the version number of the save data.
     *                Defaults to 0 if not provided.
     * @return A newly constructed instance of FRPGEntitySaveData initialized with the given parameters.
     */
    explicit FRPGEntitySaveData(const TSubclassOf<URPGEntity> EntityClass, const int32 Version = 0)
        : Version(Version), EntityClass(EntityClass)
    {
    }
};

/**
 * FRPGEntitySaveDataHandle is a structure responsible for managing and encapsulating save data related to an RPG
 * entity. It provides functionalities to retrieve entity-specific information and maintain data consistency through
 * versioning. This structure is also capable of serializing its data for efficient storage.
 */
USTRUCT(BlueprintType)
struct RPGSAVING_API FRPGEntitySaveDataHandle
{
    GENERATED_BODY()

    /**
     * Default constructor for the FRPGEntitySaveDataHandle structure.
     * Initializes an instance with default values.
     *
     * @return A default-initialized FRPGEntitySaveDataHandle object.
     */
    FRPGEntitySaveDataHandle() = default;

    /**
     * Constructs an instance of FRPGEntitySaveDataHandle using the specified entity class and version.
     * This constructor initializes the save data handle with the provided entity class type and optional version.
     *
     * @param EntityClass The class type of the RPG entity for which the save data handle will be created.
     * @param Version The version number of the save data. Defaults to 0 if not specified.
     * @return An instance of FRPGEntitySaveDataHandle initialized with the given parameters.
     */
    explicit FRPGEntitySaveDataHandle(TSubclassOf<URPGEntity> EntityClass, int32 Version = 0)
        : Data(MakeShared<FRPGEntitySaveData>(EntityClass, Version))
    {
    }

    /**
     * Retrieves the version number of the entity save data.
     *
     * This function provides access to the version information stored within the save data handle.
     * The version is used to track changes or updates to the save data format.
     *
     * @return The version number of the save data as an integer.
     */
    int32 GetVersion() const
    {
        return Data->Version;
    }

    /**
     * Retrieves the class type of the RPG entity associated with this save data handle.
     *
     * @return The class type of the RPG entity as a TSubclassOf<URPGEntity>.
     */
    TSubclassOf<URPGEntity> GetEntityClass() const
    {
        return Data->EntityClass;
    }

    /**
     * Retrieves a mutable reference to the map of required components.
     * This map associates field paths of object properties with their corresponding save data handles.
     * The data is used to manage and restore the state of components required for an RPG entity.
     *
     * @return A reference to a map where keys are TFieldPath<FObjectProperty> and values are
     * FRPGComponentSaveDataHandle.
     */
    TMap<TFieldPath<FObjectProperty>, FRPGComponentSaveDataHandle> &GetRequiredComponents() const
    {
        return Data->RequiredComponents;
    }

    /**
     * Retrieves a reference to the map containing additional component data associated with this entity.
     * The map holds key-value pairs where the key is of type FAdditionalComponentDataKey, and the value
     * is an FRPGComponentSaveDataHandle. This data can be used to manage and restore the state of additional
     * components for an RPG entity.
     *
     * @return A reference to the map of additional component data.
     */
    TMap<FAdditionalComponentDataKey, FRPGComponentSaveDataHandle> &GetAdditionalComponents() const
    {
        return Data->AdditionalComponents;
    }

    /**
     * Serializes the save data of an RPG entity using the provided archive.
     * The function ensures that the save data is processed by the FArchive system
     * and prepares it for either saving or loading to/from a persistent state.
     *
     * @param Ar The archive used for saving or loading the RPG entity save data.
     * @return True if the serialization was successful, false otherwise.
     */
    bool Serialize(FArchive &Ar)
    {
        static FRPGEntitySaveData Defaults;
        FRPGEntitySaveData::StaticStruct()->SerializeItem(Ar, &Data.Get(), &Defaults);
        return true;
    }

  private:
    TSharedRef<FRPGEntitySaveData> Data = MakeShared<FRPGEntitySaveData>();
};

/**
 * TStructOpsTypeTraits is a traits structure used to define serialization and network handling behaviors
 * for the FRPGEntitySaveDataHandle structure. These traits determine specific operations that are supported
 * or required for the FRPGEntitySaveDataHandle.
 *
 * The traits include the following flags:
 * - WithSerializer: Indicates that the FRPGEntitySaveDataHandle supports custom serialization.
 * - WithNetSerializer: Indicates whether network serialization is supported (set to false).
 * - WithNetSharedSerialization: Indicates whether shared network serialization is supported (set to false).
 */
template <>
struct TStructOpsTypeTraits<FRPGEntitySaveDataHandle> : TStructOpsTypeTraitsBase2<FRPGEntitySaveDataHandle>
{
    enum
    {
        WithSerializer = true,
        WithNetSerializer = false,
        WithNetSharedSerialization = false
    };
};