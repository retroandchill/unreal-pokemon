using UnrealSharp.CoreUObject;

namespace UnrealSharp.CommonUtilities;

public static class DateTimeExtensions
{
    extension(FDateTime dateTime)
    {
        public static FDateTime Now => UTimeBlueprintLibrary.DateTimeNow;
    }
}
