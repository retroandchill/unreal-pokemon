using Pokemon.Editor.Serialization.Pbs.Converters;

namespace Pokemon.Editor.Serialization.Pbs.Attributes;

/// <summary>
/// Represents an abstract base class for PBS scalar attributes.
/// This class provides the structure necessary to establish the type of converter
/// that will be used to handle the serialization and deserialization of scalar types
/// in the PBS schema.
/// </summary>
public abstract class PbsScalarAttribute : Attribute
{
    /// <summary>
    /// Represents the type of the converter associated with a PBS scalar attribute.
    /// This property is utilized to define the custom logic for serialization and deserialization
    /// of scalar types within the PBS schema.
    /// </summary>
    public abstract Type ConverterType { get; }
}

/// <summary>
/// Serves as the base attribute for scalar properties, parameters, or types
/// in the PBS serialization schema. This attribute provides a mechanism to
/// specify a customizable converter type that defines how the scalar data
/// is processed during serialization and deserialization.
/// </summary>
/// <typeparam name="T">The type of the converter associated with the scalar attribute.</typeparam>
[AttributeUsage(
    AttributeTargets.Property
        | AttributeTargets.Parameter
        | AttributeTargets.Class
        | AttributeTargets.Struct
        | AttributeTargets.Enum
        | AttributeTargets.Interface
)]
public sealed class PbsScalarAttribute<T> : PbsScalarAttribute
    where T : IPbsConverter
{
    /// <summary>
    /// Defines the specific type of the converter used for handling serialization
    /// and deserialization of PBS scalar data. This property enables the
    /// customization of conversion logic by associating a concrete converter
    /// implementation with the scalar attribute.
    /// </summary>
    public override Type ConverterType => typeof(T);
}
