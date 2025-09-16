using UnrealSharp.Core;
using UnrealSharp.CoreUObject;

namespace InteractiveUI.Glue.Extensions;

internal class AsyncLoadUtilities
{
    internal static UObject WorldContextObject
    {
        get
        {
            var worldContextObject = FCSManagerExporter.CallGetCurrentWorldContext();
            var worldContextHandle = FCSManagerExporter.CallFindManagedObject(worldContextObject);
            return GCHandleUtilities.GetObjectFromHandlePtr<UObject>(worldContextHandle)!;
        }
    }
}
