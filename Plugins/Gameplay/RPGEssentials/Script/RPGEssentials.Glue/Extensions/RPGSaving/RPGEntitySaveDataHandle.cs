using UnrealSharp.Core;
using UnrealSharp.RPGCore;

namespace UnrealSharp.RPGSaving;

public partial struct FRPGEntitySaveDataHandle
{
    /// <summary>
    /// Gets the version of the RPG entity save data associated with the current save data handle.
    /// The value represents a specific format or structure of the save data.
    /// This property allows compatibility checks or migration handling when dealing
    /// with different versions of save data.
    /// </summary>
    public int Version => URPGEntitySaveDataExtensions.GetVersion(this);

    /// <summary>
    /// Gets the class type of the RPG entity associated with the current save data handle.
    /// This property specifies the type of entity that can be instantiated or loaded
    /// based on the saved data, enabling entity-specific handling during loading or processing.
    /// </summary>
    public TSubclassOf<URPGEntity> EntityClass => this.GetEntityClass();

    /// <summary>
    /// Retrieves a required component's save data handle for the given property name.
    /// Throws an exception if the required component is not available.
    /// </summary>
    /// <param name="propertyName">The name of the property that corresponds to the required component.</param>
    /// <returns>The save data handle for the required component.</returns>
    /// <exception cref="System.InvalidOperationException">Thrown if the entity does not have the required component.</exception>
    public FRPGComponentSaveDataHandle GetRequiredComponent(FName propertyName)
    {
        return TryGetRequiredComponent(propertyName, out var data)
            ? data
            : throw new InvalidOperationException(
                $"Entity {EntityClass} does not have required component {propertyName}"
            );
    }

    /// <summary>
    /// Attempts to retrieve the save data handle for a required component based on the provided property name.
    /// Returns true if the component exists, otherwise false.
    /// </summary>
    /// <param name="propertyName">The name of the property that corresponds to the required component.</param>
    /// <param name="componentData">When this method returns, contains the save data handle for the specified component if it exists; otherwise, the default value.</param>
    /// <returns>True if the required component exists; otherwise, false.</returns>
    public bool TryGetRequiredComponent(FName propertyName, out FRPGComponentSaveDataHandle componentData)
    {
        return this.GetRequiredComponent(propertyName, out componentData);
    }

    /// <summary>
    /// Retrieves an additional component's save data handle for the specified component class and optional index.
    /// Throws an exception if the additional component is not found.
    /// </summary>
    /// <param name="componentClass">The class type of the additional component to retrieve.</param>
    /// <param name="index">The index of the additional component, defaulting to 0.</param>
    /// <returns>The save data handle for the additional component.</returns>
    /// <exception cref="System.InvalidOperationException">Thrown if the entity does not have the specified additional component.</exception>
    public FRPGComponentSaveDataHandle GetAdditionalComponent(TSubclassOf<URPGComponent> componentClass, int index = 0)
    {
        return TryGetAdditionalComponent(componentClass, index, out var data)
            ? data
            : throw new InvalidOperationException(
                $"Entity {EntityClass} does not have additional component {componentClass}"
            );
    }

    /// <summary>
    /// Retrieves an additional component's save data handle for the specified component class and index.
    /// </summary>
    /// <param name="index">The index of the component to retrieve, default is 0.</param>
    /// <returns>The save data handle for the specified additional component.</returns>
    /// <exception cref="System.InvalidOperationException">Thrown if the component class or index is invalid or not found.</exception>
    public FRPGComponentSaveDataHandle GetAdditionalComponent<T>(int index = 0)
        where T : URPGComponent
    {
        return GetAdditionalComponent(typeof(T), index);
    }

    /// <summary>
    /// Attempts to retrieve additional component save data for a specified component class and index.
    /// Returns a boolean indicating success or failure.
    /// </summary>
    /// <param name="componentClass">The class of the component to retrieve data for.</param>
    /// <param name="componentData">When this method returns, contains the save data handle for the requested component, if found.</param>
    /// <returns>True if the component data was successfully retrieved; otherwise, false.</returns>
    public bool TryGetAdditionalComponent(
        TSubclassOf<URPGComponent> componentClass,
        out FRPGComponentSaveDataHandle componentData
    )
    {
        return this.GetAdditionalComponent(new FAdditionalComponentDataKey(componentClass), out componentData);
    }

    /// <summary>
    /// Attempts to retrieve additional component save data for the specified component class and index.
    /// </summary>
    /// <param name="componentClass">The class type of the additional component to retrieve.</param>
    /// <param name="index">The index of the additional component, if multiple components of the same class exist.</param>
    /// <param name="componentData">When this method returns, contains the save data handle for the additional component, if found; otherwise, null.</param>
    /// <returns>True if the additional component was found; otherwise, false.</returns>
    public bool TryGetAdditionalComponent(
        TSubclassOf<URPGComponent> componentClass,
        int index,
        out FRPGComponentSaveDataHandle componentData
    )
    {
        return this.GetAdditionalComponent(new FAdditionalComponentDataKey(componentClass, index), out componentData);
    }

    /// <summary>
    /// Attempts to retrieve the save data handle of an additional component of the specified type.
    /// Returns a boolean value indicating whether the component was found.
    /// </summary>
    /// <typeparam name="T">The type of the component to retrieve. Must inherit from URPGComponent.</typeparam>
    /// <param name="componentData">When this method returns, contains the save data handle of the component if found, or a default value if not found.</param>
    /// <returns>True if the component was found; otherwise, false.</returns>
    public bool TryGetAdditionalComponent<T>(out FRPGComponentSaveDataHandle componentData)
        where T : URPGComponent
    {
        return TryGetAdditionalComponent(typeof(T), out componentData);
    }

    /// <summary>
    /// Attempts to retrieve an additional component's save data handle for the specified type and index.
    /// Returns a Boolean indicating whether the operation was successful.
    /// </summary>
    /// <typeparam name="T">The type of the component to retrieve. Must inherit from URPGComponent.</typeparam>
    /// <param name="index">The index of the additional component to retrieve.</param>
    /// <param name="componentData">When this method returns, contains the save data handle of the requested component if found, or the default value if not.</param>
    /// <returns><c>true</c> if the requested component was found; otherwise, <c>false</c>.</returns>
    public bool TryGetAdditionalComponent<T>(int index, out FRPGComponentSaveDataHandle componentData)
        where T : URPGComponent
    {
        return TryGetAdditionalComponent(typeof(T), index, out componentData);
    }
}
