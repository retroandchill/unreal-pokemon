namespace Pokemon.Editor.Serialization.Pbs.Attributes;

/// <summary>
/// An attribute used to define minimum and maximum length constraints for a property.
/// This attribute is typically applied to properties in data models to enforce
/// validation rules for the number of elements in a collection or the size of data.
/// This attribute is primarily used in conjunction with PBS serialization, where
/// field descriptors interpret the length constraints and enforce them during processing.
/// Usage of this attribute is limited to properties only.
/// </summary>
[AttributeUsage(AttributeTargets.Property)]
public class PbsLengthAttribute : Attribute
{
    /// <summary>
    /// Gets the minimum length constraint value for a property.
    /// This property indicates the smallest number of elements
    /// allowed in a collection or the minimum size of data for a property.
    /// Typically used alongside <see cref="PbsLengthAttribute"/> to enforce
    /// validations in PBS serialization.
    /// </summary>
    public int Min { get; init; }

    /// <summary>
    /// Gets the maximum length constraint value for a property.
    /// This property specifies the largest number of elements allowed in a collection
    /// or the maximum size of data for a property. It is commonly used with
    /// <see cref="PbsLengthAttribute"/> to enforce constraints during PBS serialization.
    /// </summary>
    public int Max { get; init; } = int.MaxValue;
}
