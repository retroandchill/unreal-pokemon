namespace Pokemon.Editor.Serialization.Pbs.Converters;

/// <summary>
/// Defines an interface for converting objects between their in-memory representation
/// and a PBS (Pokemon Serialization) CSV textual representation.
/// </summary>
public interface IPbsConverter
{
    /// <summary>
    /// Represents the type of object that the converter is handling or associated with.
    /// This property is typically used to identify the specific type the implementation of
    /// the IPbsConverter interface is designed to manage.
    /// </summary>
    Type Type { get; }

    /// <summary>
    /// Converts an object into a CSV textual representation based on the schema and optional section name provided.
    /// </summary>
    /// <param name="value">The object to be converted to a CSV value. This can be null.</param>
    /// <param name="schema">The scalar descriptor that defines the schema details for the object being converted.</param>
    /// <param name="sectionName">The optional name of the section associated with this CSV serialization.</param>
    /// <returns>A string representing the object's CSV format that adheres to the specified schema.</returns>
    string WriteCsvValue(object? value, PbsScalarDescriptor schema, string? sectionName);

    /// <summary>
    /// Parses a CSV textual input into its corresponding object representation based on the provided scalar descriptor and optional section name.
    /// </summary>
    /// <param name="input">The CSV formatted string to be parsed into an object.</param>
    /// <param name="scalarDescriptor">The scalar descriptor defining the schema and conversion rules for the input.</param>
    /// <param name="sectionName">The optional name of the section associated with the CSV input being parsed.</param>
    /// <returns>The object representation of the CSV input, or null if parsing fails or the value is optional and not present.</returns>
    object? GetCsvValue(string input, PbsScalarDescriptor scalarDescriptor, string? sectionName);
}

/// <summary>
/// Represents a base interface for PBS (Pokemon Serialization) converters
/// that facilitates the conversion of objects between their in-memory representations
/// and their textual PBS format.
/// </summary>
public interface IPbsConverter<T> : IPbsConverter
{
    /// <summary>
    /// Converts an object into a CSV textual representation based on the schema and optional section name provided.
    /// </summary>
    /// <param name="value">The object to be converted to a CSV value. The type of this object depends on the specific implementation of the converter.</param>
    /// <param name="schema">The scalar descriptor that defines the schema details for the object being converted.</param>
    /// <param name="sectionName">The optional name of the section associated with this CSV serialization. This may be null.</param>
    /// <returns>A string representing the object's CSV format that adheres to the specified schema.</returns>
    string WriteCsvValue(T value, PbsScalarDescriptor schema, string? sectionName);

    /// <summary>
    /// Parses a CSV value from its textual representation into its corresponding in-memory object
    /// based on the provided scalar descriptor and optional section name.
    /// </summary>
    /// <param name="input">The CSV text input to be parsed into the desired object type.</param>
    /// <param name="scalarDescriptor">The scalar descriptor defining the schema and constraints of the value to be parsed.</param>
    /// <param name="sectionName">The optional name of the section containing the CSV value.</param>
    /// <returns>The in-memory object that corresponds to the parsed CSV input.</returns>
    new T? GetCsvValue(string input, PbsScalarDescriptor scalarDescriptor, string? sectionName);
}
