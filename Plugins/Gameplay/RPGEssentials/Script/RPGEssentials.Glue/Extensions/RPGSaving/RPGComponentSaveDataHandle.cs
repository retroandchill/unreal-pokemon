using UnrealSharp.CoreUObject;
using UnrealSharp.RPGCore;

namespace UnrealSharp.RPGSaving;

public partial struct FRPGComponentSaveDataHandle
{
    public FRPGComponentSaveDataHandle(TSoftClassPtr<URPGComponent> componentClass, int version = 1)
    {
        this = URPGComponentSaveDataHandleExtensions.CreateNew(componentClass, version);
    }

    public int Version => this.GetVersion();

    public TSubclassOf<URPGComponent> ComponentClass => this.GetComponentClass();

    public UScriptStruct? SaveDataStruct => this.GetSaveDataStruct();

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
