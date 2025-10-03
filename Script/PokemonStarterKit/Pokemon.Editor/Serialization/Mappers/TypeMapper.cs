using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Model.Pbs;
using Riok.Mapperly.Abstractions;

namespace Pokemon.Editor.Serialization.Mappers;

/// <summary>
/// Provides mapping functionality between game type data models and editor serialization models.
/// </summary>
/// <remarks>
/// This static class is implemented using source generation tools to facilitate mapping operations
/// between the <c>FType</c> struct, representing game data types, and the <c>TypeInfo</c> record,
/// representing serialized editor models.
/// </remarks>
[Mapper(RequiredMappingStrategy = RequiredMappingStrategy.Target, PreferParameterlessConstructors = false)]
public static partial class TypeMapper
{
    /// <summary>
    /// Maps a <see cref="TypeInfo"/> instance to an <see cref="FType"/> instance.
    /// </summary>
    /// <param name="typeInfo">
    /// The <see cref="TypeInfo"/> instance to be mapped to <see cref="FType"/>.
    /// </param>
    /// <returns>
    /// A mapped <see cref="FType"/> instance representing the game data equivalent of the provided <see cref="TypeInfo"/>.
    /// </returns>
    public static partial FType ToType(this TypeInfo typeInfo);

    /// <summary>
    /// Maps an <see cref="FType"/> instance to a <see cref="TypeInfo"/> instance.
    /// </summary>
    /// <param name="type">
    /// The <see cref="FType"/> instance to be mapped to <see cref="TypeInfo"/>.
    /// </param>
    /// <returns>
    /// A mapped <see cref="TypeInfo"/> instance representing the serialized editor model equivalent of the provided <see cref="FType"/>.
    /// </returns>
    public static partial TypeInfo ToTypeInfo(this FType type);
}
