using UnrealSharp;
using UnrealSharp.Core;
using UnrealSharp.CoreUObject;

namespace GameDataAccessTools.Core.Utilities;

/// <summary>
/// Provides extension methods for script-based structs utilized within GameDataAccessTools.
/// </summary>
public static class ScriptStructExtensions
{
    /// <summary>
    /// Extensions for any struct that inherits from MarshalledStruct.
    /// </summary>
    /// <typeparam name="T">The type of struct</typeparam>
    extension<T>(T)
        where T : struct, MarshalledStruct<T>
    {
        /// <summary>
        /// Gets the static representation of the UScriptStruct associated with a struct derived from MarshalledStruct.
        /// </summary>
        /// <remarks>
        /// This property is used to retrieve the unmanaged representation of the structure for use in contexts where
        /// the native data structure is required, such as interop with Unreal Engine systems.
        /// </remarks>
        /// <exception cref="InvalidOperationException">
        /// Thrown when the native class pointer for the struct is null.
        /// </exception>
        /// <value>
        /// The static instance of the associated UScriptStruct.
        /// </value>
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
