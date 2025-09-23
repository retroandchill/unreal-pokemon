using UnrealSharp.RPGCore;

namespace UnrealSharp.RPGSaving;

public partial struct FRPGEntitySaveDataHandle
{
    public int Version => URPGEntitySaveDataExtensions.GetVersion(this);

    public TSubclassOf<URPGEntity> EntityClass => this.GetEntityClass();

    public FRPGComponentSaveDataHandle GetRequiredComponent(FName propertyName)
    {
        return TryGetRequiredComponent(propertyName, out var data)
            ? data
            : throw new InvalidOperationException(
                $"Entity {EntityClass} does not have required component {propertyName}"
            );
    }

    public bool TryGetRequiredComponent(
        FName propertyName,
        out FRPGComponentSaveDataHandle componentData
    )
    {
        return this.GetRequiredComponent(propertyName, out componentData);
    }

    public FRPGComponentSaveDataHandle GetAdditionalComponent(
        TSubclassOf<URPGComponent> componentClass,
        int index = 0
    )
    {
        return TryGetAdditionalComponent(componentClass, index, out var data)
            ? data
            : throw new InvalidOperationException(
                $"Entity {EntityClass} does not have additional component {componentClass}"
            );
    }

    public FRPGComponentSaveDataHandle GetAdditionalComponent<T>(int index = 0)
        where T : URPGComponent
    {
        return GetAdditionalComponent(typeof(T), index);
    }

    public bool TryGetAdditionalComponent(
        TSubclassOf<URPGComponent> componentClass,
        out FRPGComponentSaveDataHandle componentData
    )
    {
        return this.GetAdditionalComponent(
            new FAdditionalComponentDataKey(componentClass),
            out componentData
        );
    }

    public bool TryGetAdditionalComponent(
        TSubclassOf<URPGComponent> componentClass,
        int index,
        out FRPGComponentSaveDataHandle componentData
    )
    {
        return this.GetAdditionalComponent(
            new FAdditionalComponentDataKey(componentClass, index),
            out componentData
        );
    }

    public bool TryGetAdditionalComponent<T>(out FRPGComponentSaveDataHandle componentData)
        where T : URPGComponent
    {
        return TryGetAdditionalComponent(typeof(T), out componentData);
    }

    public bool TryGetAdditionalComponent<T>(
        int index,
        out FRPGComponentSaveDataHandle componentData
    )
        where T : URPGComponent
    {
        return TryGetAdditionalComponent(typeof(T), index, out componentData);
    }
}
