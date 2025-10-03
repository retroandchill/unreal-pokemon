using System.Collections.Immutable;
using System.Numerics;
using System.Reflection;
using Pokemon.Editor.Serialization.Pbs.Converters;

namespace Pokemon.Editor.Serialization.Pbs;

/// <summary>
/// Represents a numeric bounds interface, typically implemented by structures
/// that define the minimum and maximum limits for numeric value ranges.
/// </summary>
public interface INumericBounds;

/// <summary>
/// Represents a record that defines the minimum and maximum bounds for numeric values,
/// specified using a generic type that must be a value type and support numeric operations.
/// </summary>
/// <typeparam name="T">The numeric type for which the bounds are defined. Must be a struct
/// and implement the <see cref="System.Numerics.INumber{T}"/> interface.</typeparam>
/// <param name="Min">The minimum value for the numeric range.</param>
/// <param name="Max">The maximum value for the numeric range.</param>
public readonly record struct NumericBounds<T>(T? Min, T? Max) : INumericBounds
    where T : struct, INumber<T>;

/// <summary>
/// Defines the modes of repetition for processing elements, specifying how
/// repeated elements are handled during serialization or deserialization.
/// </summary>
public enum RepeatMode
{
    /// <summary>
    /// No repeat handling is performed.
    /// </summary>
    None,

    /// <summary>
    /// Allow multiple key-value pairs to be specified for the same key.
    /// </summary>
    KeyRepeat,

    /// <summary>
    /// Allow for multiple entries in the CSV entry data.
    /// </summary>
    CsvRepeat,
}

/// <summary>
/// Represents a namespace and key format used for localization in the PBS serialization framework.
/// </summary>
/// <param name="Namespace">The namespace for the localized text.</param>
/// <param name="KeyFormat">The key format for the localized text.</param>
public readonly record struct LocalizedTextNamespace(string Namespace, string KeyFormat);

/// <summary>
/// Represents a descriptor for scalar data in the PBS (Pokemon Battle System) serialization schema.
/// Defines the scalar's type, optionality, numeric bounds, and optional gameplay tag or localization constraints.
/// </summary>
public sealed record PbsScalarDescriptor(Type Type, bool IsOptional = false)
{
    /// <summary>
    /// Gets the namespace associated with the Gameplay Tag.
    /// This property defines the root namespace for categorizing gameplay tags, allowing structured
    /// and organized handling of gameplay-related identifiers within the serialization system.
    /// A null value indicates the absence of a namespace, which may affect serialization logic
    /// when working with gameplay tags.
    /// </summary>
    public string? GameplayTagNamespace { get; init; }

    /// <summary>
    /// Indicates whether a new gameplay tag should be created when using the
    /// associated descriptor during the serialization or metadata processing phase.
    /// When set to true, it ensures that new gameplay tags can be dynamically generated
    /// if they do not already exist in the predefined namespace. Otherwise, existing tags
    /// are expected to match without creating new ones.
    /// </summary>
    public bool CreateNewGameplayTag { get; init; }

    /// <summary>
    /// Defines the separator string used to delineate segments within a gameplay tag.
    /// This property enables customization of how gameplay tag strings are split into hierarchical
    /// components, facilitating structured and consistent tag processing during serialization.
    /// A null value indicates no separator, and the tag remains unaltered.
    /// </summary>
    public string? GameplayTagSeparator { get; init; }

    /// <summary>
    /// Gets the numeric bounds applicable to the scalar data described by the PBS serialization schema.
    /// This property specifies an optional range, defined by a minimum and maximum value,
    /// that numeric values must adhere to during serialization and validation processes.
    /// A null value indicates the absence of any constraints on the numeric range.
    /// </summary>
    public INumericBounds? NumericBounds { get; init; }

    /// <summary>
    /// Gets the namespace and key format utilized for handling localized text.
    /// This property specifies the namespace and the formatting pattern required to construct
    /// and organize localization keys, enabling structured management of localized text
    /// in the serialization process. A null value indicates the absence of a defined localized
    /// text namespace, which may impact the localization behavior during serialization or deserialization.
    /// </summary>
    public LocalizedTextNamespace? LocalizedTextNamespace { get; init; }

    /// <summary>
    /// Gets the scalar converter associated with the PBS Scalar Descriptor.
    /// This property provides conversion logic to map between PBS scalar values and their
    /// corresponding .NET representations. It enables custom handling of scalar types during
    /// the serialization and deserialization processes.
    /// A null value indicates that no specific converter is defined for the scalar type.
    /// </summary>
    public IPbsConverter? ScalarConverter { get; init; }
}

/// <summary>
/// Represents a PBS field descriptor, used to define metadata about a specific field
/// within a PBS schema, including its associated property, data structure, and constraints.
/// </summary>
public sealed record PbsFieldDescriptor(
    string KeyName,
    PropertyInfo TargetProperty,
    ImmutableArray<PbsScalarDescriptor> Elements
)
{
    /// <summary>
    /// Indicates whether the field descriptor represents a unique identifier within the schema.
    /// This property is used to determine if the associated field serves as a key or primary identifier
    /// in operations such as serialization, deserialization, or schema validation.
    /// </summary>
    public bool IsIdentifier { get; init; }

    /// <summary>
    /// Indicates whether the field is a row index in the context of PBS serialization.
    /// When set to true, this property designates the field as uniquely identifying
    /// a specific row's position within the structured data. This differentiates
    /// it from other fields and ensures it is treated as an index marker
    /// during compilation and serialization operations.
    /// </summary>
    public bool IsRowIndex { get; init; }

    /// <summary>
    /// Defines the repeat mode of elements associated with this field.
    /// This property determines how the field's data is structured and
    /// serialized, particularly when dealing with repeated or scalar elements.
    /// It plays a crucial role in serialization logic by specifying if repeated
    /// values are handled as keys, CSV, or not repeated at all.
    /// </summary>
    public RepeatMode Repeat { get; init; }

    /// <summary>
    /// Indicates whether the field is scalar.
    /// A field is considered scalar if it consists of a single element and does not have a CSV repeat mode.
    /// </summary>
    public bool IsScalar => Elements.Length == 1 && Repeat != RepeatMode.CsvRepeat;

    /// <summary>
    /// Specifies the minimum length constraint for a collection or value when serialized using the PBS schema.
    /// This property defines the least number of elements or characters required for a valid field value.
    /// It is primarily used to enforce structural and validation rules during data serialization and
    /// deserialization processes, ensuring the integrity of serialized data.
    /// A value of zero signifies no minimum length restriction.
    /// </summary>
    public int MinLength { get; init; }

    /// <summary>
    /// Gets or sets the maximum allowable length for the field content.
    /// This property enforces a constraint on the number of elements or characters
    /// that can be assigned to the field, ensuring the content adheres to defined limits.
    /// A value of <c>int.MaxValue</c> indicates no explicit upper bound.
    /// </summary>
    public int MaxLength { get; init; }
}

/// <summary>
/// Represents a schema for managing field descriptors within a PBS serialization context.
/// Provides functionality to add and retrieve descriptors associated with specific key names.
/// </summary>
public sealed class PbsSchema
{
    private readonly Dictionary<string, PbsFieldDescriptor> _byBaseKey = new();

    /// <summary>
    /// Gets the collection of PBS field descriptors stored within this schema.
    /// The Fields property maps key names to their corresponding field descriptors,
    /// providing metadata definitions about PBS schema fields such as their structure,
    /// constraints, and relation to properties in the target data model.
    /// </summary>
    public IReadOnlyDictionary<string, PbsFieldDescriptor> Fields => _byBaseKey;

    /// <summary>
    /// Adds a PBS field descriptor to the current schema, associating it with its key name.
    /// </summary>
    /// <param name="descriptor">
    /// The <see cref="PbsFieldDescriptor"/> to be added to the schema. This defines the metadata and
    /// configuration for a specific field within the PBS serialization context.
    /// </param>
    /// <returns>
    /// The current <see cref="PbsSchema"/> instance, allowing method chaining.
    /// </returns>
    public PbsSchema Add(PbsFieldDescriptor descriptor)
    {
        _byBaseKey.Add(descriptor.KeyName, descriptor);
        return this;
    }

    /// <summary>
    /// Attempts to retrieve a <see cref="PbsFieldDescriptor"/> associated with the specified base key.
    /// </summary>
    /// <param name="baseKey">
    /// The key name to locate the associated <see cref="PbsFieldDescriptor"/> in the schema.
    /// </param>
    /// <param name="field">
    /// When this method returns, contains the <see cref="PbsFieldDescriptor"/> associated with the specified base key,
    /// if found; otherwise, the default value for the type of the field parameter.
    /// </param>
    /// <returns>
    /// A boolean value indicating whether a <see cref="PbsFieldDescriptor"/> matching the specified base key was found.
    /// </returns>
    public bool TryGetField(string baseKey, out PbsFieldDescriptor field)
    {
        return _byBaseKey.TryGetValue(baseKey, out field!);
    }
}
