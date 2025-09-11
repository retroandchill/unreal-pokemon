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

public abstract class PbsConverterBase<T> : IPbsConverter<T>
{
    public Type Type => typeof(T);

    string IPbsConverter.WriteCsvValue(
        object? value,
        PbsScalarDescriptor schema,
        string? sectionName
    )
    {
        ArgumentNullException.ThrowIfNull(value);
        return WriteCsvValue((T)value, schema, sectionName);
    }

    public abstract string WriteCsvValue(T value, PbsScalarDescriptor schema, string? sectionName);

    object? IPbsConverter.GetCsvValue(
        string input,
        PbsScalarDescriptor scalarDescriptor,
        string? sectionName
    )
    {
        return GetCsvValue(input, scalarDescriptor, sectionName);
    }

    public abstract T? GetCsvValue(
        string input,
        PbsScalarDescriptor scalarDescriptor,
        string? sectionName
    );
}
