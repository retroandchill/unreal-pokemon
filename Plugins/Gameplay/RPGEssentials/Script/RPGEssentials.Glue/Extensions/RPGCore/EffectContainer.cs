using UnrealSharp.Core;
using UnrealSharp.Core.Marshallers;
using UnrealSharp.CoreUObject;

namespace UnrealSharp.RPGCore;

public partial class UEffectContainer
{
    private MapReadOnlyMarshaller<FName, FInstancedStruct>? _effectsMarshaller;

    /// <summary>
    /// Represents a read-only map of effects in the container.
    /// Each effect is identified using a unique key of type FName and holds data as FInstancedStruct.
    /// This property allows access to the current state of effects stored in the container.
    /// </summary>
    public TMapReadOnly<FName, FInstancedStruct> Effects
    {
        get
        {
            _effectsMarshaller ??= new MapReadOnlyMarshaller<FName, FInstancedStruct>(
                EffectData_NativeProperty,
                BlittableMarshaller<FName>.ToNative,
                BlittableMarshaller<FName>.FromNative,
                StructMarshaller<FInstancedStruct>.ToNative,
                StructMarshaller<FInstancedStruct>.FromNative
            );
            return _effectsMarshaller.FromNative(IntPtr.Add(NativeObject, EffectData_Offset), 0);
        }
    }

    /// <summary>
    /// Provides an enumerable collection of keys representing the effects in the container.
    /// Each key corresponds to an effect identified by a unique FName, allowing access to the effects stored in the container.
    /// </summary>
    public IEnumerable<FName> EffectKeys => EffectData.Keys;

    /// <summary>
    /// Retrieves the effect data associated with the specified effect name.
    /// Throws an exception if the specified effect data cannot be cast to the requested type.
    /// </summary>
    /// <typeparam name="T">The type of the data to retrieve. Must be a struct and implement <see cref="MarshalledStruct{Self}"/>.</typeparam>
    /// <param name="effect">The name of the effect whose data should be retrieved.</param>
    /// <returns>
    /// The data associated with the specified effect, cast to the requested type.
    /// </returns>
    /// <exception cref="InvalidOperationException">Thrown if the data cannot be cast to the specified type.</exception>
    public T GetEffectData<T>(FName effect)
        where T : struct, MarshalledStruct<T>
    {
        return TryGetEffectData<T>(effect, out var data)
            ? data
            : throw new InvalidOperationException($"Data is not of type: {typeof(T).Name}");
    }

    /// <summary>
    /// Retrieves the effect data associated with the specified effect name or returns the provided default value
    /// if the effect data does not exist or cannot be cast to the requested type.
    /// </summary>
    /// <typeparam name="T">The type of the data to retrieve. Must be a struct and implement <see cref="MarshalledStruct{Self}"/>.</typeparam>
    /// <param name="effect">The name of the effect whose data should be retrieved.</param>
    /// <param name="alternative">The default value to return if the effect data cannot be retrieved or cast.</param>
    /// <returns>
    /// The data associated with the specified effect, cast to the requested type, or the provided default value if no data is found.
    /// </returns>
    public T GetEffectDataOrDefault<T>(FName effect, T alternative = default)
        where T : struct, MarshalledStruct<T>
    {
        return TryGetEffectData<T>(effect, out var data) ? data : alternative;
    }
}
