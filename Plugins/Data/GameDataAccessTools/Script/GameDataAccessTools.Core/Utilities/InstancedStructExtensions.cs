using GameDataAccessTools.Core.Views;
using UnrealSharp;
using UnrealSharp.Core;
using UnrealSharp.CoreUObject;
using UnrealSharp.Interop;

namespace GameDataAccessTools.Core.Utilities;

public static class InstancedStructExtensions
{
    extension(FInstancedStruct instancedStruct)
    {
        public UScriptStruct? StructType => instancedStruct.ScriptStruct;
    }

    extension(StructView<FInstancedStruct> structView)
    {
        public UScriptStruct? StructType
        {
            get
            {
                unsafe
                {
                    var structData = (FInstancedStructData*)structView.NativePtr;
                    var nativeStruct = FInstancedStructExporter.CallGetNativeStruct(
                        ref *structData
                    );
                    var handle = FCSManagerExporter.CallFindManagedObject(nativeStruct);
                    return GCHandleUtilities.GetObjectFromHandlePtr<UScriptStruct>(handle);
                }
            }
        }

        public bool IsA(UScriptStruct scriptStruct)
        {
            return structView.IsA(scriptStruct.NativeObject);
        }

        public bool IsA<T>()
            where T : struct, MarshalledStruct<T>
        {
            return structView.IsA(T.GetNativeClassPtr());
        }

        private bool IsA(IntPtr scriptStruct)
        {
            unsafe
            {
                var structData = (FInstancedStructData*)structView.NativePtr;
                var nativeStruct = FInstancedStructExporter.CallGetNativeStruct(ref *structData);
                return nativeStruct == scriptStruct;
            }
        }

        public T Get<T>()
            where T : struct, MarshalledStruct<T>
        {
            return structView.TryGet(out T value)
                ? value
                : throw new InvalidOperationException(
                    $"Failed to get {typeof(T).Name} from instanced struct."
                );
        }

        public bool TryGet<T>(out T value)
            where T : struct, MarshalledStruct<T>
        {
            if (!structView.IsA<T>())
            {
                value = default;
                return false;
            }

            unsafe
            {
                var structData = (FInstancedStructData*)structView.NativePtr;
                value = T.FromNative(FInstancedStructExporter.CallGetMemory(ref *structData));
                return true;
            }
        }
    }
}
