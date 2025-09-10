using System.Collections.Immutable;
using System.Numerics;
using System.Reflection;
using Pokemon.Editor.Serialization.Pbs.Converters;

namespace Pokemon.Editor.Serialization.Pbs;

public interface INumericBounds;

public readonly record struct NumericBounds<T>(T? Min, T? Max) : INumericBounds
    where T : struct, INumber<T>;

public enum RepeatMode
{
    None,
    KeyRepeat,
    CsvRepeat,
}

public readonly record struct LocalizedTextNamespace(string Namespace, string KeyFormat);

public sealed record PbsScalarDescriptor(Type Type, bool IsOptional = false)
{
    public string? GameplayTagNamespace { get; init; }

    public bool CreateNewGameplayTag { get; init; }

    public string? GameplayTagSeparator { get; init; }

    public INumericBounds? NumericBounds { get; init; }

    public LocalizedTextNamespace? LocalizedTextNamespace { get; init; }

    public IPbsConverter? ScalarConverter { get; init; }
}

public sealed record PbsFieldDescriptor(
    string KeyName,
    PropertyInfo TargetProperty,
    ImmutableArray<PbsScalarDescriptor> Elements
)
{
    public bool IsIdentifier { get; init; }
    public bool IsRowIndex { get; init; }

    public RepeatMode Repeat { get; init; }

    public bool IsScalar => Elements.Length == 1 && Repeat != RepeatMode.CsvRepeat;
}

public sealed class PbsSchema
{
    private readonly Dictionary<string, PbsFieldDescriptor> _byBaseKey = new();

    public IReadOnlyDictionary<string, PbsFieldDescriptor> Fields => _byBaseKey;

    public PbsSchema Add(PbsFieldDescriptor descriptor)
    {
        _byBaseKey.Add(descriptor.KeyName, descriptor);
        return this;
    }

    public bool TryGetField(string baseKey, out PbsFieldDescriptor field)
    {
        return _byBaseKey.TryGetValue(baseKey, out field!);
    }
}
