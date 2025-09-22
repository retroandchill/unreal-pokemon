using UnrealSharp.RPGCore;

namespace UnrealSharp.RPGSaving;

public partial struct FRPGEntitySaveDataHandle
{
    public int Version => URPGEntitySaveDataExtensions.GetVersion(this);

    public TSoftClassPtr<URPGEntity> EntityClass => this.GetEntityClass();

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
        TSoftClassPtr<URPGComponent> componentClass
    )
    {
        return TryGetAdditionalComponent(componentClass, out var data)
            ? data
            : throw new InvalidOperationException(
                $"Entity {EntityClass} does not have additional component {componentClass}"
            );
    }

    public FRPGComponentSaveDataHandle GetAdditionalComponent(
        TSoftClassPtr<URPGComponent> componentClass,
        int index
    )
    {
        return TryGetAdditionalComponent(componentClass, index, out var data)
            ? data
            : throw new InvalidOperationException(
                $"Entity {EntityClass} does not have additional component {componentClass} at index {index}"
            );
    }

    public bool TryGetAdditionalComponent(
        TSoftClassPtr<URPGComponent> componentClass,
        out FRPGComponentSaveDataHandle componentData
    )
    {
        return this.GetAdditionalComponent(
            new FAdditionalComponentDataKey(componentClass),
            out componentData
        );
    }

    public bool TryGetAdditionalComponent(
        TSoftClassPtr<URPGComponent> componentClass,
        int index,
        out FRPGComponentSaveDataHandle componentData
    )
    {
        return this.GetAdditionalComponent(
            new FAdditionalComponentDataKey(componentClass, index),
            out componentData
        );
    }
}
