using System.Collections;
using System.Collections.Immutable;
using System.Diagnostics.CodeAnalysis;
using System.Numerics;
using System.Reflection;
using Pokemon.Editor.Serialization.Pbs.Attributes;
using Pokemon.Editor.Serialization.Pbs.Converters;
using UnrealSharp;
using UnrealSharp.GameplayTags;

namespace Pokemon.Editor.Serialization.Pbs;

public static class PbsMetamodel
{
    private static readonly ImmutableArray<Type> ScalarTypes =
    [
        typeof(bool),
        typeof(sbyte),
        typeof(short),
        typeof(int),
        typeof(long),
        typeof(byte),
        typeof(ushort),
        typeof(uint),
        typeof(ulong),
        typeof(float),
        typeof(double),
        typeof(decimal),
        typeof(string),
        typeof(FName),
        typeof(FText),
        typeof(FGameplayTag),
    ];

    public static PbsSchema GetSchema<T>()
    {
        var schema = new PbsSchema();
        foreach (
            var property in typeof(T)
                .GetProperties(BindingFlags.Public | BindingFlags.Instance)
                .Where(p => p is { CanRead: true, CanWrite: true })
        )
        {
            var lengthAttribute = property.GetCustomAttribute<PbsLengthAttribute>();

            schema.Add(
                new PbsFieldDescriptor(
                    property.GetKeyName(),
                    property,
                    property.GetScalarDescriptors()
                )
                {
                    IsIdentifier = property.GetCustomAttribute<PbsKeyAttribute>() is not null,
                    IsRowIndex = property.GetCustomAttribute<PbsIndexAttribute>() is not null,
                    Repeat = property.GetRepeatMode(),
                    MinLength = lengthAttribute?.Min ?? 0,
                    MaxLength = lengthAttribute?.Max ?? int.MaxValue,
                }
            );
        }
        return schema;
    }

    private static RepeatMode GetRepeatMode(this PropertyInfo property)
    {
        if (property.GetCustomAttribute<PbsAllowMultipleAttribute>() is not null)
        {
            return RepeatMode.KeyRepeat;
        }

        return property.TryGetCollectionType(out _) ? RepeatMode.CsvRepeat : RepeatMode.None;
    }

    private static string GetKeyName(this PropertyInfo property)
    {
        return property.GetCustomAttribute<PbsNameAttribute>()?.Name ?? property.Name;
    }

    private static PbsScalarDescriptor CreateScalarDescriptor(PropertyInfo property)
    {
        return CreateScalarDescriptor(property.PropertyType, property);
    }

    private static PbsScalarDescriptor CreateScalarDescriptor(
        Type type,
        ICustomAttributeProvider property,
        bool isOptional = false
    )
    {
        var gameplayTag = property.GetCustomAttribute<PbsGameplayTag>();
        return new PbsScalarDescriptor(type, isOptional)
        {
            GameplayTagNamespace = gameplayTag?.Namespace,
            CreateNewGameplayTag = gameplayTag is not null && gameplayTag.Create,
            GameplayTagSeparator = gameplayTag?.Separator,
            NumericBounds = property.CreateNumericBounds(),
            LocalizedTextNamespace = property.CreateLocalizedTextNamespace(),
            ScalarConverter = GetScalarConverterTypes(type, property)
                .Select(converterType => (IPbsConverter)Activator.CreateInstance(converterType)!)
                .FirstOrDefault(converter => converter.Type.IsAssignableFrom(type)),
        };
    }

    private static T? GetCustomAttribute<T>(this ICustomAttributeProvider provider)
        where T : Attribute
    {
        return provider.GetCustomAttributes(typeof(T), false).FirstOrDefault() as T;
    }

    private static IEnumerable<Type> GetScalarConverterTypes(
        Type elementType,
        ICustomAttributeProvider property
    )
    {
        var propertyLevelConverter = property
            .GetCustomAttribute<PbsScalarAttribute>()
            ?.ConverterType;

        if (propertyLevelConverter is not null)
            yield return propertyLevelConverter;

        var typeLevelConverter = elementType
            .GetCustomAttribute<PbsScalarAttribute>()
            ?.ConverterType;
        if (typeLevelConverter is not null)
            yield return typeLevelConverter;
    }

    private static INumericBounds? CreateNumericBounds(this ICustomAttributeProvider property)
    {
        var numberType = property switch
        {
            PropertyInfo p => p.PropertyType,
            ParameterInfo p => p.ParameterType,
            _ => throw new InvalidOperationException(),
        };
        if (
            !numberType.IsValueType
            || !numberType
                .GetInterfaces()
                .Any(i => i.IsGenericType && i.GetGenericTypeDefinition() == typeof(INumber<>))
        )
        {
            return null;
        }

        return (INumericBounds?)
            typeof(PbsMetamodel)
                .GetMethod(
                    nameof(CreateNumericBoundsInternal),
                    BindingFlags.Static | BindingFlags.NonPublic
                )!
                .MakeGenericMethod(numberType)
                .Invoke(null, [property]);
    }

    private static NumericBounds<T> CreateNumericBoundsInternal<T>(
        this ICustomAttributeProvider property
    )
        where T : struct, INumber<T>
    {
        var rangeAttribute = property.GetCustomAttribute<PbsRangeAttribute<T>>();
        return rangeAttribute is not null
            ? new NumericBounds<T>(rangeAttribute.Min, rangeAttribute.Max)
            : default;
    }

    private static LocalizedTextNamespace? CreateLocalizedTextNamespace(
        this ICustomAttributeProvider property
    )
    {
        var localizedTextAttribute = property.GetCustomAttribute<PbsLocalizedTextAttribute>();
        return localizedTextAttribute is not null
            ? new LocalizedTextNamespace(
                localizedTextAttribute.Namespace,
                localizedTextAttribute.KeyFormat
            )
            : null;
    }

    private static ImmutableArray<PbsScalarDescriptor> GetScalarDescriptors(
        this PropertyInfo property
    )
    {
        if (
            property.PropertyType.IsScalarType()
            || property.GetCustomAttribute<PbsScalarAttribute>() is not null
        )
        {
            return [CreateScalarDescriptor(property)];
        }

        if (property.TryGetCollectionType(out var elementType))
        {
            return elementType.IsScalarType()
                ? [CreateScalarDescriptor(elementType, property)]
                : GetComplexTypeDescriptors(elementType);
        }

        return GetComplexTypeDescriptors(property.PropertyType);
    }

    private static ImmutableArray<PbsScalarDescriptor> GetComplexTypeDescriptors(Type elementType)
    {
        var constructor = elementType.GetComplexTypeConstructor();
        return
        [
            .. constructor
                .GetParameters()
                .Select(p => CreateScalarDescriptor(p.ParameterType, p, p.HasDefaultValue)),
        ];
    }

    public static bool IsScalarType(this Type type)
    {
        if (type.IsGenericType && type.GetGenericTypeDefinition() == typeof(Nullable<>))
        {
            type = type.GetGenericArguments()[0];
        }

        return type.IsEnum
            || ScalarTypes.Contains(type)
            || type.GetCustomAttribute<PbsScalarAttribute>() is not null;
    }

    private static ConstructorInfo GetComplexTypeConstructor(this Type type)
    {
        return type.GetConstructors(BindingFlags.Public | BindingFlags.Instance)
            .Single(c => c.GetParameters().Length > 0);
    }

    public static bool TryGetCollectionFactory(
        this Type type,
        [NotNullWhen(true)] out Func<IEnumerable<object?>, object>? factory
    )
    {
        if (type == typeof(FGameplayTagContainer))
        {
            factory = x => new FGameplayTagContainer(x.OfType<FGameplayTag>().ToList());
            return true;
        }

        if (type.IsGenericType && type.GetGenericTypeDefinition() == typeof(IReadOnlyList<>))
        {
            var elementType = type.GetGenericArguments()[0];
            factory = x =>
            {
                var targetList = (IList)
                    Activator.CreateInstance(typeof(List<>).MakeGenericType(elementType))!;
                foreach (var element in x)
                {
                    targetList.Add(element);
                }

                return targetList;
            };
            return true;
        }

        factory = null;
        return false;
    }

    public static bool TryGetCollectionType(
        this PropertyInfo property,
        [NotNullWhen(true)] out Type? elementType
    )
    {
        // For now only worry about FGameplayTagContainer and IReadOnlyList
        if (property.PropertyType == typeof(FGameplayTagContainer))
        {
            elementType = typeof(FGameplayTag);
            return true;
        }

        if (
            property.PropertyType.IsGenericType
            && property.PropertyType.GetGenericTypeDefinition() == typeof(IReadOnlyList<>)
        )
        {
            elementType = property.PropertyType.GetGenericArguments()[0];
            return true;
        }

        elementType = null;
        return false;
    }
}
