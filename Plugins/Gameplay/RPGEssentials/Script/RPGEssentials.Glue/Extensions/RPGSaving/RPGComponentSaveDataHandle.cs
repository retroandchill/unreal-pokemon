using UnrealSharp.RPGCore;

namespace UnrealSharp.RPGSaving;

public partial struct FRPGComponentSaveDataHandle
{
    public static FRPGComponentSaveDataHandle New<T>(
        TSoftClassPtr<URPGComponent> componentClass,
        T data,
        int version = 1
    )
        where T : struct, MarshalledStruct<T>
    {
        return URPGComponentSaveDataHandleExtensions.CreateNew(componentClass, data, version);
    }

    public int Version => this.GetVersion();

    public TSoftClassPtr<URPGComponent> ComponentClass => this.GetComponentClass();

    public T GetData<T>()
        where T : struct, MarshalledStruct<T>
    {
        return TryGetData<T>(out var data)
            ? data
            : throw new InvalidOperationException($"Data is not of type: {typeof(T).Name}");
    }

    public bool TryGetData<T>(out T data)
        where T : struct, MarshalledStruct<T>
    {
        return this.GetData(out data);
    }

    public void SetData<T>(T data)
        where T : struct, MarshalledStruct<T>
    {
        URPGComponentSaveDataHandleExtensions.SetData(this, data);
    }
}
