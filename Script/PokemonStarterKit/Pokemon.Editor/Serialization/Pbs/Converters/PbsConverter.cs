namespace Pokemon.Editor.Serialization.Pbs.Converters;

public interface IPbsConverter
{
    Type Type { get; }

    string WriteCsvValue(object? value, PbsScalarDescriptor schema, string? sectionName);

    object? GetCsvValue(string input, PbsScalarDescriptor scalarDescriptor, string? sectionName);
}

public interface IPbsConverter<T> : IPbsConverter
{
    string WriteCsvValue(T value, PbsScalarDescriptor schema, string? sectionName);

    new T? GetCsvValue(string input, PbsScalarDescriptor scalarDescriptor, string? sectionName);
}
