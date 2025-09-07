using UnrealSharp;
using UnrealSharp.Core;
using UnrealSharp.CoreUObject;

namespace GameDataAccessTools.Core.Utilities;

public static class ScriptStructExtensions
{
    extension<T>(T)
        where T : struct, MarshalledStruct<T>
    {
        public static UScriptStruct StaticStruct
        {
            get
            {
                var nativeStruct = T.GetNativeClassPtr();

                if (nativeStruct == IntPtr.Zero)
                {
                    throw new InvalidOperationException("Native class pointer is null!");
                }

                var handle = FCSManagerExporter.CallFindManagedObject(nativeStruct);
                return GCHandleUtilities.GetObjectFromHandlePtr<UScriptStruct>(handle)!;
            }
        }
    }
}
