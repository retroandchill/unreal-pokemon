using System.Numerics;

namespace Pokemon.Editor.Serialization.Pbs.Attributes;

/// <summary>
/// An attribute to specify a numeric range constraint for properties or parameters.
/// </summary>
/// <typeparam name="T">
/// The numeric type of the range. This must be a struct that implements <see cref="System.Numerics.INumber{T}"/>.
/// </typeparam>
/// <remarks>
/// This attribute can be used to define a minimum and/or maximum bound for numeric values.
/// It supports generic numeric types while ensuring compile-time type safety. Both the minimum
/// and maximum values are optional and default to null if not provided.
/// </remarks>
/// <example>
/// Apply this attribute to specify constraints on numeric properties or parameters.
/// </example>
[AttributeUsage(AttributeTargets.Property | AttributeTargets.Parameter)]
public class PbsRangeAttribute<T> : Attribute
    where T : struct, INumber<T>
{
    /// <summary>
    /// Create a new instance of the <see cref="PbsRangeAttribute{T}"/> class.
    /// </summary>
    /// <param name="min">The minimum value.</param>
    public PbsRangeAttribute(T min)
    {
        Min = min;
    }

    /// <summary>
    /// Create a new instance of the <see cref="PbsRangeAttribute{T}"/> class.
    /// </summary>
    /// <param name="min">The minimum value</param>
    /// <param name="max">The maximum value</param>
    public PbsRangeAttribute(T min, T max)
    {
        Min = min;
        Max = max;
    }

    /// <summary>
    /// Create a new instance of the <see cref="PbsRangeAttribute{T}"/> class.
    /// </summary>
    /// <param name="min">The minimum value</param>
    /// <param name="max">The maximum value</param>
    public PbsRangeAttribute(object? min, T max)
    {
        Min = min is T t ? t : default;
        Max = max;
    }

    /// <summary>
    /// Gets the minimum value for the numeric range constraint.
    /// </summary>
    /// <remarks>
    /// The <c>Min</c> property specifies the lower bound of the numeric range.
    /// If no minimum value is provided during the creation of the range attribute,
    /// this property will be null. The type of this property ensures compile-time type safety
    /// for the specified numeric type.
    /// </remarks>
    public T? Min { get; }

    /// <summary>
    /// Gets the maximum value for the numeric range constraint.
    /// </summary>
    /// <remarks>
    /// The <c>Max</c> property represents the upper bound of the numeric range.
    /// If no maximum value is provided during the creation of the range attribute,
    /// this property will be null. The type of this property ensures compile-time type safety
    /// for the specified numeric type.
    /// </remarks>
    public T? Max { get; }
}
