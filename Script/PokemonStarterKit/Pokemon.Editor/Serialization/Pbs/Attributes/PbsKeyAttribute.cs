namespace Pokemon.Editor.Serialization.Pbs.Attributes;

/// <summary>
/// An attribute used to mark a property as the key for serialization purposes in the Pokémon Battle Simulator (PBS).
/// </summary>
/// <remarks>
/// The <see cref="PbsKeyAttribute"/> is applied to properties within data records or models to designate them as
/// primary identifiers during serialization and deserialization operations.
/// </remarks>
[AttributeUsage(AttributeTargets.Property)]
public class PbsKeyAttribute : Attribute;
