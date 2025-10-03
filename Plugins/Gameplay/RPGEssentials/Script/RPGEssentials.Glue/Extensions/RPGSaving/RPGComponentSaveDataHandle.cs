using UnrealSharp.CoreUObject;
using UnrealSharp.RPGCore;

namespace UnrealSharp.RPGSaving;

public partial struct FRPGComponentSaveDataHandle
{
    /// <summary>
    /// Creates a new save data handle for the given component class.
    /// </summary>
    /// <param name="componentClass">The component class to create</param>
    /// <param name="version">The version of the save data</param>
    public FRPGComponentSaveDataHandle(TSoftClassPtr<URPGComponent> componentClass, int version = 1)
    {
        this = URPGComponentSaveDataHandleExtensions.CreateNew(componentClass, version);
    }

    /// <summary>
    /// Gets the version of the save data associated with the RPG component handle.
    /// </summary>
    public int Version => this.GetVersion();

    /// <summary>
    /// Gets the class type of the RPG component associated with this save data handle.
    /// </summary>
    public TSubclassOf<URPGComponent> ComponentClass => this.GetComponentClass();

    /// <summary>
    /// Retrieves the struct that defines the save data schema for the RPG component associated with this handle.
    /// </summary>
    public UScriptStruct? SaveDataStruct => this.GetSaveDataStruct();

    /// <summary>
    /// Retrieves the save data of the specified type from the handle.
    /// </summary>
    /// <typeparam name="T">The data structure type that implements MarshalledStruct for deserialization.</typeparam>
    /// <returns>The save data of the specified type.</returns>
    /// <exception cref="InvalidOperationException">Thrown if the data is not of the specified type.</exception>
    public T GetData<T>()
        where T : struct, MarshalledStruct<T>
    {
        return TryGetData<T>(out var data)
            ? data
            : throw new InvalidOperationException($"Data is not of type: {typeof(T).Name}");
    }

    /// <summary>
    /// Attempts to retrieve the save data for the specified type from the component save handle.
    /// </summary>
    /// <typeparam name="T">The struct type of save data to retrieve, which must implement the MarshalledStruct interface.</typeparam>
    /// <param name="data">The output parameter that will contain the retrieved data if available.</param>
    /// <returns>A boolean value indicating whether the data was successfully retrieved.</returns>
    public bool TryGetData<T>(out T data)
        where T : struct, MarshalledStruct<T>
    {
        return this.GetData(out data);
    }

    /// <summary>
    /// Sets the save data for the specified component.
    /// </summary>
    /// <param name="data">The data to set for the component. This must be a structure that implements the MarshalledStruct<T> interface.</param>
    /// <typeparam name="T">The type of the data structure, which must be a struct that implements the MarshalledStruct<T> interface.</typeparam>
    public void SetData<T>(T data)
        where T : struct, MarshalledStruct<T>
    {
        URPGComponentSaveDataHandleExtensions.SetData(this, data);
    }
}
