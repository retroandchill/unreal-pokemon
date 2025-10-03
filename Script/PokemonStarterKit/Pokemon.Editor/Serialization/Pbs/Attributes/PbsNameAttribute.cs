namespace Pokemon.Editor.Serialization.Pbs.Attributes;

/// <summary>
/// An attribute used to specify a custom name for a property or parameter during serialization.
/// </summary>
/// <remarks>
/// This attribute is designed for use within the PBS (Pokémon Battle Simulator) serialization system
/// to provide a specific name mapping for properties or parameters. It is commonly applied to denote
/// the field name as expected in serialized data, differing from the C# property or parameter name.
/// </remarks>
/// <param name="name">The custom name to assign to the property or parameter.</param>
[AttributeUsage(AttributeTargets.Property | AttributeTargets.Parameter)]
public class PbsNameAttribute(string name) : Attribute
{
    /// <summary>
    /// Gets the name assigned to the property or parameter during serialization.
    /// </summary>
    /// <remarks>
    /// Represents the custom name applied using the <see cref="PbsNameAttribute"/> attribute.
    /// The value of this property is used to map serialized data fields to corresponding properties
    /// or parameters within the PBS (Pokémon Battle Simulator) serialization system.
    /// </remarks>
    public string Name { get; } = name;
}
