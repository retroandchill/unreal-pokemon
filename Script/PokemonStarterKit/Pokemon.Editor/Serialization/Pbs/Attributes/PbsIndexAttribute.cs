namespace Pokemon.Editor.Serialization.Pbs.Attributes;

/// <summary>
/// An attribute used to designate a property as a row index index.
/// </summary>
/// <remarks>
/// This attribute is applied to properties that represent a positional index in a PBS data structure.
/// It is commonly used during serialization and deserialization processes to map properties
/// to their respective rows in PBS files.
/// </remarks>
[AttributeUsage(AttributeTargets.Property)]
public class PbsIndexAttribute : Attribute;
