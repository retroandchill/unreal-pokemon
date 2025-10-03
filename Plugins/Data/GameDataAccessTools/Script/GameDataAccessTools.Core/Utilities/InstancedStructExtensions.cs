using GameDataAccessTools.Core.Views;
using UnrealSharp;
using UnrealSharp.Core;
using UnrealSharp.CoreUObject;
using UnrealSharp.Interop;

namespace GameDataAccessTools.Core.Utilities;

/// <summary>
/// Provides extension methods for operations involving InstancedStruct.
/// </summary>
public static class InstancedStructExtensions
{
    /// <summary>
    /// Extensions for FInstancedStruct.
    /// </summary>
    /// <param name="instancedStruct"></param>
    extension(FInstancedStruct instancedStruct)
    {
        /// <summary>
        /// Represents the type of a UScriptStruct associated with a given FInstancedStruct.
        /// </summary>
        /// <remarks>
        /// This property provides access to the UScriptStruct type of the underlying instanced struct,
        /// allowing inspection of the structural type definition.
        /// </remarks>
        public UScriptStruct? StructType => instancedStruct.ScriptStruct;
    }

    /// <summary>
    /// Extensions for <see cref="StructView{T}"/>.
    /// </summary>
    /// <param name="structView"></param>
    extension(StructView<FInstancedStruct> structView)
    {
        /// <summary>
        /// Gets the UScriptStruct type of the associated FInstancedStruct.
        /// </summary>
        /// <remarks>
        /// This property retrieves the structural type definition of the instanced struct,
        /// providing access to the underlying UScriptStruct metadata.
        /// </remarks>
        public UScriptStruct? StructType
        {
            get
            {
                unsafe
                {
                    var structData = (FInstancedStructData*)structView.NativePtr;
                    var nativeStruct = FInstancedStructExporter.CallGetNativeStruct(ref *structData);
                    var handle = FCSManagerExporter.CallFindManagedObject(nativeStruct);
                    return GCHandleUtilities.GetObjectFromHandlePtr<UScriptStruct>(handle);
                }
            }
        }

        /// <summary>
        /// Determines whether the struct view is of the specified UScriptStruct type.
        /// </summary>
        /// <param name="scriptStruct">The UScriptStruct to check against the struct view.</param>
        /// <returns>True if the struct view is of the specified type; otherwise, false.</returns>
        public bool IsA(UScriptStruct scriptStruct)
        {
            return structView.IsA(scriptStruct.NativeObject);
        }

        /// <summary>
        /// Determines whether the struct view contains a structure of the specified type.
        /// </summary>
        /// <typeparam name="T">The type of struct to check against the struct view.</typeparam>
        /// <returns>True if the struct view contains a structure of the specified type; otherwise, false.</returns>
        public bool IsA<T>()
            where T : struct, MarshalledStruct<T>
        {
            return structView.IsA(T.GetNativeClassPtr());
        }

        /// <summary>
        /// Determines whether the struct view is of the specified UScriptStruct type.
        /// </summary>
        /// <param name="scriptStruct">The UScriptStruct to check against the struct view.</param>
        /// <returns>True if the struct view is of the specified type; otherwise, false.</returns>
        private bool IsA(IntPtr scriptStruct)
        {
            unsafe
            {
                var structData = (FInstancedStructData*)structView.NativePtr;
                var nativeStruct = FInstancedStructExporter.CallGetNativeStruct(ref *structData);
                return nativeStruct == scriptStruct;
            }
        }

        /// <summary>
        /// Retrieves the structure of the specified type from the struct view.
        /// </summary>
        /// <typeparam name="T">The type of struct to retrieve from the struct view.</typeparam>
        /// <returns>The structure of type <typeparamref name="T"/> contained within the struct view.</returns>
        /// <exception cref="InvalidOperationException">
        /// Thrown if the struct view does not contain a structure of the specified type.
        /// </exception>
        public T Get<T>()
            where T : struct, MarshalledStruct<T>
        {
            return structView.TryGet(out T value)
                ? value
                : throw new InvalidOperationException($"Failed to get {typeof(T).Name} from instanced struct.");
        }

        /// <summary>
        /// Attempts to retrieve a structure of the specified type from the struct view.
        /// </summary>
        /// <typeparam name="T">The type of struct to retrieve.</typeparam>
        /// <param name="value">The retrieved value if the operation is successful; otherwise, the default value for the type.</param>
        /// <returns>True if the struct view contains a structure of the specified type and the value is successfully retrieved; otherwise, false.</returns>
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
