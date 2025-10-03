namespace Pokemon.Editor.Serialization.Pbs.Attributes;

/// <summary>
/// Specifies that the decorated property can have multiple entries.
/// This attribute enables handling of properties that can represent
/// collections or repeated entries within a given context, such as
/// deserializing and managing elements that support multiple instances.
/// </summary>
/// <remarks>
/// Applicable to properties to indicate repeatable entries.
/// It interacts with the metadata model functionality to define
/// behaviors like collection handling or repeat modes.
/// </remarks>
[AttributeUsage(AttributeTargets.Property)]
public class PbsAllowMultipleAttribute : Attribute;
